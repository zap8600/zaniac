// Font
// #include "VGA8.h"

// Boot params
#include "../kernel/sysparam.h"

// EFI data
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

typedef enum efimemtype_t {
    efireservedmem,
    efiloadercode,
    efiloaderdata,
    efibscode,
    efibsdata,
    efirtscode,
    efirtsdata,
    eficonvetmem,
    efiunusablemem,
    efiacpireclaimmem,
    efiacpimemnvs,
    efimemmappedio,
    efimemmappedioport,
    efipalcode,
    efipersistmem,
    efiunacceptedmem,
    efimaxmemtype
} efimemtype_t;

typedef struct efimemdesc_t {
    unsigned int type;
    unsigned int pad;
    unsigned long int physicalstart;
    unsigned long int virtualstart;
    unsigned long int numofpages;
    unsigned long int attr;
} efimemdesc_t;

typedef unsigned long int (*efihandleprot_t)(void* handle, efiguid_t* prot, void** interface);
typedef unsigned long int (*efiallocpool_t)(efimemtype_t memtype, unsigned long int size, void** ret);
typedef unsigned long int (*efifreepool_t)(void* buf);
//typedef unsigned long int (*efiwaitforevent_t)(unsigned long int numofevents, void** event, unsigned long int* index);
typedef unsigned long int (*efilocprot_t)(efiguid_t* prot, void* registration, void** interface);
typedef unsigned long int (*efigetmemmap_t)(unsigned long int* memmapsize, efimemdesc_t* memmap, unsigned long int* mapkey, unsigned long int* descsize, unsigned int* descversion);
typedef unsigned long int (*efiexitbootservices_t)(void* imghandle, unsigned long int mapkey);

typedef struct efibservices_t {
    efitableheader_t header;
    void* raisetpl;
    void* restoretpl;

    void* allocatepages;
    void* freepages;
    efigetmemmap_t getmemorymap;
    efiallocpool_t allocatepool;
    efifreepool_t freepool;

    void* eventnotify;
    void* createevent;
    void* settimer;
    void* waitforevent;
    void* signalevent;
    void* closeevent;

    void* installprotif;
    void* reinstallprotif;
    void* uninstallprotif;
    efihandleprot_t handleprot;
    void* pchandleprot;
    void* registerprotnot;
    void* lochandle;
    void* locdevpath;
    void* installconftable;

    void* loadimage;
    void* startimage;
    void* exit;
    void* unloadimage;
    efiexitbootservices_t exitbootservices;

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

typedef struct efidevpath_t {
    unsigned char type;
    unsigned char subtype;
    unsigned char length[2];
} efidevpath_t;

#define LIPGUID {0x5B1B31A1, 0x9562, 0x11d2, {0x8E, 0x3F, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B}}

typedef struct efiloadedimageprot_t {
    unsigned int revision;
    void* parenthandle;
    void* systab;
    void* devhanle;
    efidevpath_t* filepath;
    void* reserved;
    unsigned int loadoptsize;
    void* loadopt;
    void* imgbase;
    unsigned long int imgsize;
    efimemtype_t imgcodetype;
    efimemtype_t imgdatatype;
} efiloadedimageprot_t;

#define EFIFILEINFOGUID {0x9576e92, 0x6d3f, 0x11d2, {0x8e, 0x39, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b}}

typedef struct efifileinfo_t {
    unsigned long int size;
    unsigned long int filesize;
    unsigned long int physicalsize;
    efitime_t createtime;
    efitime_t lastaccesstime;
    efitime_t modificationtime;
    unsigned long int attr;
    unsigned short int filename[262];
} efifileinfo_t;

typedef struct efifilehandle_t efifilehandle_t;

typedef unsigned long int (*efifileopen_t)(efifilehandle_t* file, efifilehandle_t** newhandle, unsigned short int* filename, unsigned long int openmode, unsigned long int attr);
typedef unsigned long int (*efifileclose_t)(efifilehandle_t* file);
typedef unsigned long int (*efifileread_t)(efifilehandle_t* file, unsigned long int* bufsize, void* buf);
typedef unsigned long int (*efigetinfo_t)(efifilehandle_t* file, efiguid_t* infotype, unsigned long int* bufsize, void* buf);

typedef struct efifilehandle_t {
    unsigned long int revision;
    efifileopen_t open;
    efifileclose_t close;
    void* delete;
    efifileread_t read;
    void* write;
    void* getpos;
    void* setpos;
    efigetinfo_t getinfo;
    void* setinfo;
    void* flush;
} efifilehandle_t;

#define SFSGUID {0x964e5b22, 0x6459, 0x11d2, {0x8e, 0x39, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b}}

typedef unsigned long int (*efiopenvolume_t)(void* this, efifilehandle_t** root);

typedef struct efisfsprot_t {
    unsigned long int revision;
    efiopenvolume_t openvolume;
} efisfsprot_t;

#define EFIFILEMODEREAD 0x1

// I/O
/*
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
*/


//Screen
/*
efigop_t* gop = (void*)0;
void initdisplay(efisystemtable_t* systab) {
    // TODO: Add error handling
    efiguid_t gopguid = GOPGUID;
    systab->bservices->locprot(&gopguid, (void*)0, (void**)&gop);
    gop->setmode(gop, 0);
}
*/

/*
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
*/

// Keyboard
/*
efisimpletextinput_t* input = (void*)0;
char getcharacter(unsigned char block) {
    unsigned long int status = 0;
    efiinputkey_t key = {0};
    if(block) {
        // The WaitForKey event doesn't seem to be properly set up on all platforms
        while((status = input->readkeystroke(input, &key)) == 6) {}
    }
    // Convert Enter key (13) to \n (10)
    if(key.unicodech == 13) {
        return '\n';
    }
    return (char)(key.unicodech);
}
*/

// Support

#define EFIERROR(a) (((signed long int) a) < 0)

void* memset(void* s, int c, unsigned long int n) {
    unsigned char* b = (unsigned char*)s;
    for(unsigned long int i = 0; i < n; i++) {
        b[i] = (unsigned char)c;
    }
    return s;
}

void* memcpy(void* d, void* s, unsigned long int n) {
    unsigned char* dst = (unsigned char*)d;
    const unsigned char* src = (const unsigned char*)s;
    for(unsigned long int i = 0; i < n; i++) {
        dst[i] = src[i];
    }
    return d;
}

/*
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
*/

// Main code
/*
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
*/
#define PTLOAD 1

typedef struct elf64ehdr_t {
    unsigned char ident[16];
    unsigned short int type;
    unsigned short int machine;
    unsigned int version;
    unsigned long int entry;
    unsigned long int phoff;
    unsigned long int shoff;
    unsigned int flags;
    unsigned short int ehsize;
    unsigned short int phentsize;
    unsigned short int phnum;
    unsigned short int shentsize;
    unsigned short int shnum;
    unsigned short int shstrndx;
} elf64ehdr_t;

typedef struct elf64phdr_t {
    unsigned int ptype;
    unsigned int flags;
    unsigned long int offset;
    unsigned long int vaddr;
    unsigned long int paddr;
    unsigned long int filesize;
    unsigned long int memsize;
    unsigned long int align;
} elf64phdr_t;

const unsigned short int kernelfilename[12] = {'\\', 'z', 'a', 'n', 'i', 'a', 'c', '.', 'e', 'l', 'f', 0};
efifilehandle_t filedata = {0};

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

    // Setup

    efiloadedimageprot_t* lip = (void*)0;
    efiguid_t lipguid = LIPGUID;
    systab->bservices->handleprot(image, &lipguid, (void**)lip);

    efifilehandle_t* rootdir = (void*)0;
    efisfsprot_t* sfs = (void*)0;
    efiguid_t sfsguid = SFSGUID;
    systab->bservices->handleprot(lip->devhanle, &sfsguid, (void**)&sfs);
    rootdir = sfs->openvolume(sfs, &rootdir);

    efifilehandle_t* kernelfile = &filedata;
    rootdir->open(rootdir, &kernelfile, kernelfilename, EFIFILEMODEREAD, 0);

    efifileinfo_t kernelfileinfo;
    efiguid_t fileinfoguid = EFIFILEINFOGUID;
    unsigned long int fileinfosize = sizeof(kernelfileinfo);
    kernelfile->getinfo(kernelfile, &fileinfoguid, &fileinfosize, &kernelfileinfo);

    void* kerneldata = (void*)0;
    systab->bservices->allocatepool(lip->imgdatatype, kernelfileinfo.filesize, &kerneldata);
    kernelfile->read(kernelfile, &(kernelfileinfo.filesize), kerneldata);
    kernelfile->close(kernelfile);

    efigop_t* gop = (void*)0;
    efiguid_t gopguid = GOPGUID;
    systab->bservices->locprot(&gopguid, (void*)0, (void**)&gop);
    gop->setmode(gop, 0);

    sysparam_t bootparams = {0};
    bootparams.framebuffer = gop->mode->fbbase;
    bootparams.hres = gop->mode->info->hres;
    bootparams.vres = gop->mode->info->vres;
    bootparams.pitch = gop->mode->infor->pixperscanline;

    elf64ehdr_t *elf = (elf64ehdr_t*)kerneldata;
    elf64phdr_t *phdr = (void*)0;
    int i;
    for(phdr = (elf64phdr_t*)(kerneldata + elf->phoff), i = 0; i < elf->phnum; i++, phdr = (elf64phdr_t*)(((unsigned char*)phdr) + elf->phentsize)) {
        if(phdr->ptype == PTLOAD) {
            memcpy((void*)(phdr->vaddr), kerneldata + phdr->offset, phdr->filesize);
            memset((void*)(phdr->paddr + phdr->filesize), 0, phdr->memsize - phdr->filesize);
        }
    }

    void* entry = elf->entry;

    systab->bservices->freepool(kerneldata);

    efimemdesc_t* memmap = (void*)0;
    unsigned long int count = 3; // Three tries
    unsigned long int memmapsize = 0;
    unsigned long int mapkey = 0;
    unsigned long int descsize = 0;
    unsigned long int status = 0;
    while(count--) {
        status = systab->bservices->getmemorymap(&memmapsize, memmap, &mapkey, &descsize, (void*)0);
        if(status != (0x8000000000000000 | (unsigned int)(5))) break;
        status = systab->bservices->exitbootservices(image, mapkey);
    }

    (*((void(* __attribute__((sysv_abi)))(sysparam_t*))(entry)))(&bootparams);

    /*
    wstrscr("Hello, world!\n");
    input = systab->conin;

    wstrscr("Firmware Vendor: ");
    unsigned short wc = 0;
    for(unsigned long int i = 0; (wc = systab->fwvendor[i]); i++) {
        wchscr((char)wc);
    }
    wchscr('\n');

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

    while(1) {
        char c = getcharacter(1);
        wchscr(c);
    }
    */

    return 0;
}
