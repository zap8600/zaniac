// Font
#include "VGA8.h"


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


typedef unsigned long int (*efiwaitforevent_t)(unsigned long int numofevents, void** event, unsigned long int* index);
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
    efiwaitforevent_t waitforevent;
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
    void* installmultipleprotif;
    void* uninstallmultipleprotif;
    void* caluclatecrc32;
} efibservices_t;

typedef struct efiinputkey_t {
    unsigned short scancode;
    unsigned short unicodech;
} efiinputkey_t;

typedef unsigned long int (*efireset_t)(void* this, unsigned char extendedverification);
typedef unsigned long int (*efireadkeystroke_t)(void* this, efiinputkey_t* key);

typedef struct efisimpletextinput_t {
    efireset_t reset;
    efireadkeystroke_t readkeystroke;
    void* event;
} efisimpletextinput_t;

typedef struct efisystemtable_t {
    efitableheader_t header;
    unsigned short* fwvendor;
    unsigned int fwrevision;
    void* coninhandle;
    efisimpletextinput_t* conin;
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
    unsigned int* fbbase;
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

void wchcom1(const char c) {
    while(!(inb(COM1 + 5) & 0x20)) {}
    outb(COM1, c);
}

void wstrcom1(const char* s) {
    char c = 0;
    while((c = *s++)) {
        wchcom1(c);
    }
}


//Screen
efigop_t* gop = (void*)0;
void initdisplay(efisystemtable_t* systab) {
    // TODO: Add error handling
    efiguid_t gopguid = GOPGUID;
    systab->bservices->locprot(&gopguid, (void*)0, (void**)&gop);
    gop->setmode(gop, 0);
}

unsigned int cx = 0;
unsigned int cy = 0;
void wchscr(const char c) {
    if(c == '\n') {
        cx = 0;
        cy += 16;
    } else {
        if((gop->mode->info->hres - cx) < 8) {
            cx = 0;
            cy += 16;
        }
        for(unsigned int cb = 0; cb < 16; cb++) {
            const unsigned char ch = VGA8_F16[(c * 16) + cb];
            for(unsigned char shift = 0; shift < 8; shift++) {
                if(ch & (0x80 >> shift)) {
                    gop->mode->fbbase[((cy + cb) * gop->mode->info->pixperscanline) + (cx + shift)] = 0xffffffff;
                } else {
                    gop->mode->fbbase[((cy + cb) * gop->mode->info->pixperscanline) + (cx + shift)] = 0x00000000;
                }
            }
        }
        cx += 8;
    }
}

void wstrscr(const char* s) {
    char c = 0;
    while((c = *s++)) {
        wchscr(c);
    }
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

unsigned int abs(signed int num) {
    if(num < 0) {
        return (unsigned int)(-num);
    } else {
        return (unsigned int)num;
    }
}

char* numtostr(unsigned int val, unsigned int base) {
    unsigned char neg = 0;
    if(((signed int)val) < 0) {
        neg = 1;
        val = abs(val);
    }
    static char buf[32] = {0};
    if(!val) {
        buf[30] = '0';
        return &buf[30];
    }
    unsigned int i = 30;
    for(; val && i; i--, val /= base) {
        buf[i] = "0123456789abcdef"[val % base];
    }
    if(neg) {
        buf[i] = '-';
        return &buf[i];
    } else {
        return &buf[i + 1];
    }
}


// Main code
const char *months[] = {
    "January",
    "Febuary",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December"
};

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

    // Display test
    initdisplay(systab);
    wstrscr("Hello, world!\n");

    wstrscr("Time of boot: ");
    // Test for accessing UEFI functions
    efitime_t time = {0};
    efitimecap_t timecap = {0};
    systab->rtservices->gettime(&time, &timecap);
    wstrscr(months[time.month - 1]);
    wchscr(' ');
    char* timevalstr = numtostr(time.day, 10);
    wstrscr(timevalstr);
    wstrscr(", ");
    timevalstr = numtostr(time.year, 10);
    wstrscr(timevalstr);
    wstrscr(" at ");
    timevalstr = numtostr(time.hour, 10);
    wstrscr(timevalstr);
    wchscr(':');
    timevalstr = numtostr(time.minute, 10);
    wstrscr(timevalstr);
    wchscr(':');
    timevalstr = numtostr(time.second, 10);
    wstrscr(timevalstr);
    wchscr('\n');

    //while(1) {
        unsigned long int index = 0;
        efiinputkey_t inkey = {0};
        unsigned long int status = systab->bservices->waitforevent(1, &(systab->conin->event), &index);
        //if(ERROR(status)) {
            char* errorstatusstr = numtostr(status, 10);
            wstrcom1("Error: ");
            wstrcom1(errorstatusstr);
            wchcom1('\n');
            //break;
        //}
        //wstrcom1("Got key!\n");
        /*
        systab->conin->readkeystroke(systab->conin, &inkey);
        wstrcom1("Got key\n");
        //wchscr((char)(inkey.unicodech));
        wchcom1((char)(inkey.unicodech));
        */
    //}

    wstrcom1("Left main loop, halting...\n");
    while(1) {}

    /*
    // COM1 test
    wstrcom1("Hello, world!\n");

    // Get GOP data
    efigop_t* gop = (void*)0;
    efiguid_t gopguid = GOPGUID;
    unsigned long int status = 0;
    status = systab->bservices->locprot(&gopguid, (void*)0, (void**)&gop);
    if(!ERROR(status) && gop) {
        status = gop->setmode(gop, 0);
        if(ERROR(status)) {
            wstrcom1("Failed to set video mode\n");
            return 0;
        }
        char* datastr = (void*)0;
        datastr = numtostr(gop->mode->info->hres, 10);
        wstrcom1("Horizontal resolution: ");
        wstrcom1(datastr);
        wchcom1('\n');
        datastr = numtostr(gop->mode->info->vres, 10);
        wstrcom1("Vertical resolution: ");
        wstrcom1(datastr);
        wchcom1('\n');
        datastr = numtostr(gop->mode->info->pixperscanline, 10);
        wstrcom1("Pixels per scan line: ");
        wstrcom1(datastr);
        wchcom1('\n');
        datastr = numtostr(gop->mode->info->pixformat, 10);
        wstrcom1("Pixel format: ");
        wstrcom1(datastr);
        wchcom1('\n');

        unsigned int cx = 0;
        unsigned int cy = 0;
        for(unsigned int i = 0; i < VGA8_F16_len; i += 16) {
            if(((gop->mode->info->hres - 1) - cx) < 8) {
                cx = 0;
                cy += 16;
            }
            const unsigned char bitmask = 0x80;
            for(unsigned int cc = 0; cc < 16; cc++) {
                const unsigned char curb = VGA8_F16[i + cc];
                for(unsigned int cb = 0; cb < 8; cb++) {
                    if((curb & (bitmask >> cb))) {
                        gop->mode->fbbase[((cy + cc) * gop->mode->info->pixperscanline) + (cx + cb)] = 0xffffffff;
                    } else {
                        gop->mode->fbbase[((cy + cc) * gop->mode->info->pixperscanline) + (cx + cb)] = 0x00000000;
                    }
                }
            }
            cx += 8;
        }
        while(1) {}
    }
    */
    return 0;
}
