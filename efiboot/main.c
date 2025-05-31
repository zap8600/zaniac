// EFI system table data
typedef struct efitableheader_t {
    unsigned long int signature;
    unsigned int revision;
    unsigned int headersize;
    unsigned int crc32;
    unsigned int reserved;
} efitableheader_t;

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
    void* rtservices;
    void* bservices;
    unsigned long int numoftableents;
    void* conftable;
} efisystemtable_t;

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

int wctomb(char* s, unsigned short u) {
    int r = 0;
    if(u < 0x80) {
        *s = u;
        r = 1;
    } else if(u < 0x800) {
        *(s + 0) = ((u >> 6) & 0x1F) | 0xC0;
        *(s + 1) = (u & 0x3F) | 0x80;
        r = 2;
    } else {
        *(s + 0) = ((u >> 12) & 0x0F) | 0xE0;
        *(s + 1) = ((u >> 6) & 0x3F) | 0x80;
        *(s + 2) = (u & 0x3F) | 0x80;
        r = 3;
    }
    return r;
}

unsigned long int wcstombs(char* s, unsigned short* pwcs, unsigned long int n) {
    int r;
    char* og = s;
    if(!s || !pwcs || !*pwcs) return 0;
    while((*pwcs && ((unsigned long int)(s - og + 4))) < n) {
        r = wctomb(s, *pwcs);
        if(r < 0) return (unsigned long int)(-1);
        pwcs++;
        s += r;
    }
    *s = 0;
    return (unsigned long int)(s - og);
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
    wstrcom1("Hello, world!\n");
    /*
    char fwvendstr[1024] = {0};
    wcstombs(fwvendstr, systab->fwvendor, sizeof(fwvendstr));
    wstrcom1("Firmware vendor: ");
    wstrcom1(fwvendstr);
    wchcom1('\n');
    */
    return 0;
}
