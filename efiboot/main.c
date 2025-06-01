// EFI system table data
typedef struct efitableheader_t {
    unsigned long int signature;
    unsigned int revision;
    unsigned int headersize;
    unsigned int crc32;
    unsigned int reserved;
} efitableheader_t;

typedef struct efitime_t {
    unsigned short year;
    unsigned char month;
    unsigned char day;
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
    unsigned char pad1;
    unsigned int nanosecond;
    signed int timezone;
    unsigned char daylight;
    unsigned char pad2;
} efitime_t;

typedef struct efitimecap_t {
    unsigned int resolution;
    unsigned int accuracy;
    unsigned char settozero;
} efitimecap_t;

typedef unsigned long int (*efigettime_t)(efitime_t* time, efitimecap_t* cap);

typedef struct efirtservices_t {
    efitableheader_t header;
    efigettime_t gettime;
} efirtservices_t;

typedef struct efiguid_t {
    unsigned int data1;
    unsigned short data2;
    unsigned short data3;
    unsigned char data4[8];
} efiguid_t;

typedef unsigned long int (*efilocprot_t)(efiguid_t* prot, void* registration, void** interface);

typedef struct efibservices_t {
    efitableheader_t header;
    void* raisetpl;
    void* restoretpl;

    void* allocatepages;
    void* freepages;
    void* getmemorymap;
    void* allocatepool;
    void* freepool;

    void* eventnotify;
    void* createevent;
    void* settimer;
    void* waitforevent;
    void* signalevent;
    void* closeevent;

    void* installprotif;
    void* reinstallprotif;
    void* uninstallprotif;
    void* handleprot;
    void* pchandleprot;
    void* registerprotnot;
    void* lochandle;
    void* locdevpath;
    void* installconftable;

    void* loadimage;
    void* startimage;
    void* exit;
    void* unloadimage;
    void* exitbootservices;

    void*getnexthimonocnt;
    void* stall;
    void* setwatchdogtimer;

    void* connectcont;
    void* disconnectcont;

    void* openprot;
    void* closeprot;
    void* openprotinfo;

    void* protsperhandle;
    void* lochanlebuf;
    efilocprot_t locprot;
} efibservices_t;

typedef struct efisystemtable_t {
    efitableheader_t header;
    unsigned short* fwvendor;
    unsigned int fwrevision;
    void* coninhandle;
    void* conin;
    void* conouthandle;
    void* conout;
    void* conerrhandle;
    void* stderr;
    efirtservices_t* rtservices;
    efibservices_t* bservices;
    unsigned long int numoftableents;
    void* conftable;
} efisystemtable_t;

#define GOPGUID {0x9042a9de, 0x23dc, 0x4a38, {0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a}}

typedef enum efigoppixformat_t {
    rgbreserved,
    bgrreserved,
    bitmask,
    bltonly,
    formatmax
} efigoppixformat_t;

typedef struct efigoppixbitmask_t {
    unsigned int rmask;
    unsigned int gmask;
    unsigned int bluemask;
    unsigned int reservedmask;
} efigoppixbitmask_t;

typedef struct efigopmodeinfo_t {
    unsigned int version;
    unsigned int hres;
    unsigned int vres;
    efigoppixformat_t pixformat;
    efigoppixbitmask_t pixinfo;
    unsigned int pixperscanline;
} efigopmodeinfo_t;

typedef struct efigopmode_t {
    unsigned int maxmode;
    unsigned int mode;
    efigopmodeinfo_t* info;
    unsigned long int sizeofinfo;
    unsigned long int fbbase;
    unsigned long int fbsize;
} efigopmode_t;

typedef unsigned long int (*efiquerymode_t)(void* this, unsigned int modenum, unsigned long int* sizeofinfo, efigopmodeinfo_t** info);
typedef unsigned long int (*efisetmode_t)(void* this, unsigned int modenum);

typedef struct efigop_t {
    efiquerymode_t querymode;
    efisetmode_t setmode;
    void* blt;
    efigopmode_t* mode;
} efigop_t;


// I/O
#define COM1 0x3f8

static inline void outb(unsigned short port, unsigned char val) {
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile ( "inb %w1, %b0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}

void wstrcom1(const char* s) {
    char c = 0;
    while((c = *s++)) {
        while(!(inb(COM1 + 5) & 0x20)) {}
        outb(COM1, c);
    }
}

void wchcom1(const char c) {
    while(!(inb(COM1 + 5) & 0x20)) {}
    outb(COM1, c);
}


// Support
#define ERROR(a) (((signed long int)a) < 0)

void* memset(void* s, int c, unsigned long int n) {
    unsigned char* b = (unsigned char*)s;
    for(unsigned long int i = 0; i < n; i++) {
        b[i] = (unsigned char)c;
    }
    return s;
}


char* numtostr(unsigned int val, unsigned int base) {
    static char buf[32] = {0};
    unsigned int i = 30;
    for(; val && i; i--, val /= base) {
        buf[i] = "0123456789abcdef"[val % base];
    }
    return &buf[i + 1];
}


// Main code
void fizzbuzz(void) {
    wstrcom1("Fizz Buzz for 100 numbers:\n");
    for(unsigned int i = 1; i <= 100; i++) {
        if(!(i % 3)) {
            if(!(i % 5)) {
                wstrcom1("fizz-buzz");
            } else {
                wstrcom1("fizz");
            }
        } else if(!(i % 5)) {
            wstrcom1("buzz");
        } else {
            wstrcom1(numtostr(i, 10));
        }
        wchcom1('\n');
    }
}

unsigned long int inituefi(void* image, efisystemtable_t* systab) {
    // Set up SSE
    __asm__ __volatile__ (
        "    movq %cr0, %rax\n"
        "    andb $0xF1, %al\n"
        "    movq %rax, %cr0\n"
        "    movq %cr4, %rax\n"
        "    orw $3 << 9, %ax\n"
        "    mov %rax, %cr4\n"
    );

    // COM1 test
    wstrcom1("Hello, world!\n");

    // Test for accessing UEFI functions
    efitime_t time = {0};
    efitimecap_t timecap = {0};
    systab->rtservices->gettime(&time, &timecap);
    wstrcom1("The year is ");
    char* yearstr = numtostr((unsigned long int)time.year, 10);
    wstrcom1(yearstr);
    wchcom1('\n');

    // Get GOP data
    efigop_t* gop = (void*)0;
    efiguid_t gopguid = GOPGUID;
    unsigned long int status = 0;
    status = systab->bservices->locprot(&gopguid, (void*)0, (void**)&gop);
    if(!ERROR(status) && gop) {
        status = gop->setmode(gop, 0);
    }
    return 0;
}
