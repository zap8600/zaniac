#ifndef _BOOT_EFI_H
#define _BOOT_EFI_H 1

#if defined(__x86_64__)
#define EFIAPI __attribute__((ms_abi))
#else
#define EFIAPI
#endif

#define EFIERR(a) (0x8000000000000000 | ((unsigned int)a))

// EFI data
typedef struct efitableheader_t {
    unsigned long long signature;
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

typedef unsigned long long (EFIAPI *efigettime_t)(efitime_t* time, efitimecap_t* cap);

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
    unsigned long long physicalstart;
    unsigned long long virtualstart;
    unsigned long long numofpages;
    unsigned long long attr;
} efimemdesc_t;

typedef enum efialloctype_t {
    efiallocany,
    efimaxaddr,
    efiaddr,
    efimaxalloctype
} efialloctype_t;

typedef unsigned long long (EFIAPI *efiallocpages_t)(efialloctype_t type, efimemtype_t memtype, unsigned long long pages, void* mem);
typedef unsigned long long (EFIAPI *efifreepages_t)(unsigned long long mem, unsigned long long pages);
typedef unsigned long long (EFIAPI *efihandleprot_t)(void* handle, efiguid_t* prot, void** interface);
typedef unsigned long long (EFIAPI *efiallocpool_t)(efimemtype_t memtype, unsigned long long size, void** ret);
typedef unsigned long long (EFIAPI *efifreepool_t)(void* buf);
typedef unsigned long long (EFIAPI *efilocprot_t)(efiguid_t* prot, void* registration, void** interface);
typedef unsigned long long (EFIAPI *efigetmemmap_t)(unsigned long long* memmapsize, efimemdesc_t* memmap, unsigned long long* mapkey, unsigned long long* descsize, unsigned int* descversion);
typedef unsigned long long (EFIAPI *efiexitbootservices_t)(void* imghandle, unsigned long long mapkey);

typedef struct efibservices_t {
    efitableheader_t header;
    void* raisetpl;
    void* restoretpl;

    efiallocpages_t allocatepages;
    efifreepages_t freepages;
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

typedef unsigned long long (EFIAPI *efireset_t)(void* this, unsigned char extendedverification);
typedef unsigned long long (EFIAPI *efireadkeystroke_t)(void* this, efiinputkey_t* key);

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
    unsigned long long numoftableents;
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
    unsigned long long sizeofinfo;
    unsigned int* fbbase;
    unsigned long long fbsize;
} efigopmode_t;

typedef unsigned long long (EFIAPI *efiquerymode_t)(void* this, unsigned int modenum, unsigned long long* sizeofinfo, efigopmodeinfo_t** info);
typedef unsigned long long (EFIAPI *efisetmode_t)(void* this, unsigned int modenum);

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
    unsigned long long imgsize;
    efimemtype_t imgcodetype;
    efimemtype_t imgdatatype;
} efiloadedimageprot_t;

#define EFIFILEINFOGUID {0x9576e92, 0x6d3f, 0x11d2, {0x8e, 0x39, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b}}

typedef struct efifileinfo_t {
    unsigned long long size;
    unsigned long long filesize;
    unsigned long long physicalsize;
    efitime_t createtime;
    efitime_t lastaccesstime;
    efitime_t modificationtime;
    unsigned long long attr;
    unsigned short int filename[262];
} efifileinfo_t;

typedef struct efifilehandle_t efifilehandle_t;

typedef unsigned long long (EFIAPI *efifileopen_t)(efifilehandle_t* file, efifilehandle_t** newhandle, unsigned short int* filename, unsigned long long openmode, unsigned long long attr);
typedef unsigned long long (EFIAPI *efifileclose_t)(efifilehandle_t* file);
typedef unsigned long long (EFIAPI *efifileread_t)(efifilehandle_t* file, unsigned long long* bufsize, void* buf);
typedef unsigned long long (EFIAPI *efigetinfo_t)(efifilehandle_t* file, efiguid_t* infotype, unsigned long long* bufsize, void* buf);

typedef struct efifilehandle_t {
    unsigned long long revision;
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

typedef unsigned long long (EFIAPI *efiopenvolume_t)(void* this, efifilehandle_t** root);

typedef struct efisfsprot_t {
    unsigned long long revision;
    efiopenvolume_t openvolume;
} efisfsprot_t;

#define EFIFILEMODEREAD 0x1

#endif