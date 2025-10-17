// Font
#include "../font/VGA8.h"

// Boot params
#include "../kernel/sysparam.h"

// EFI
#include "efi.h"

// https://www.youtube.com/watch?v=mHh2-ixF9Yk&t=1031s

//Screen
efigop_t* gop = (void*)0;

void clearscr() {
    for(unsigned int y = 0; y < gop->mode->info->vres; y++) {
        for(unsigned int x = 0; x < gop->mode->info->hres; x++) {
            gop->mode->fbbase[(y * gop->mode->info->pixperscanline) + x] = 0x00000000;
        }
    }
}

unsigned int cx = 0;
unsigned int cy = 0;
void wchscr(const char c) {
    if(c == '\n') {
        cx = 0;
        if((cy + 16) >= gop->mode->info->vres) {
            clearscr();
            cy = 0;
        } else {
            cy += 16;
        }
    } else {
        if((gop->mode->info->hres - cx) < 8) {
            cx = 0;
            if((cy + 16) >= gop->mode->info->vres) {
                clearscr();
                cy = 0;
            } else {
                cy += 16;
            }
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

#define EFIERROR(a) (((signed long int) a) < 0)

void* memset(void* s, int c, unsigned long long n) {
    unsigned char* b = (unsigned char*)s;
    for(unsigned long long i = 0; i < n; i++) {
        b[i] = (unsigned char)c;
    }
    return s;
}

void* memcpy(void* d, void* s, unsigned long long n) {
    unsigned char* dst = (unsigned char*)d;
    const unsigned char* src = (const unsigned char*)s;
    for(unsigned long long i = 0; i < n; i++) {
        dst[i] = src[i];
    }
    return d;
}

// const char numcharbuf[17];

char* ptrtostr(unsigned long long ptr) {
    static char ptrstrbuf[19] = {0};
    memcpy(ptrstrbuf, "0x", 2);
    for(unsigned long long i = 0, j = 15; i < 16; i++, j--) {
        ptrstrbuf[2 + j] = "0123456789abcdef"[(ptr >> (i * 4)) & 0x0f];
    }
    return &(ptrstrbuf[0]);
}

#define PTNULL 0
#define PTLOAD 1

typedef struct elf64ehdr_t {
    unsigned char ident[16];
    unsigned short int type;
    unsigned short int machine;
    unsigned int version;
    unsigned long long entry;
    unsigned long long phoff;
    unsigned long long shoff;
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
    unsigned long long offset;
    unsigned long long vaddr;
    unsigned long long paddr;
    unsigned long long filesize;
    unsigned long long memsize;
    unsigned long long align;
} elf64phdr_t;

unsigned short int kernelfilename[12] = {'\\', 'z', 'a', 'n', 'i', 'a', 'c', '.', 'e', 'l', 'f', 0};
efifilehandle_t filedata = {0};

unsigned long long pml4[512] __attribute__((aligned(4096))) = {0}; // root
unsigned long long pdpt[512] __attribute__((aligned(4096))) = {0}; // root
unsigned long long pd[512] __attribute__((aligned(4096))) = {0}; // for memmap
unsigned long long pd1[512] __attribute__((aligned(4096))) = {0}; // for kernel
unsigned long long pd2[512] __attribute__((aligned(4096))) = {0}; // for framebuffer
unsigned long long pt[512] __attribute__((aligned(4096))) = {0}; // for kernel

#define PAGE4K (4 * 1024)
#define PAGE2M (2 * 1024 * 1024)
#define PAGE1G (1 * 1024 * 1024 * 1024)

#define PAGEPS 0x80 // PS flag
#define PAGERW 0x2 // Read/Write flag
#define PAGEP 0x1 // Present flag

void sortmemmap(efimemdesc_t* memmap, unsigned long long amt) {
    // TODO: Sort the memmap
    efimemdesc_t tempentry = {0};
    unsigned long long smallindex = amt - 1;
    unsigned long long stopper = 0;
    while(stopper < amt) {
        for(unsigned long long i = 0, j = amt - 1; (i < amt) && (j >= stopper); i++, j--) {
            if(memmap[j].physicalstart < memmap[smallindex].physicalstart) {
                smallindex = j;
            }
        }
        
        if(stopper != (amt - 1)) {
            // Swaps entries
            tempentry = memmap[smallindex];
            memmap[smallindex] = memmap[stopper];
            memmap[stopper] = tempentry;
        }
        stopper++;
        smallindex = amt - 1;
    }
}

void outb(unsigned short int port, unsigned char value) {
    asm volatile("outb %b0, %w1" : : "a"(value), "Nd"(port) : "memory");
}

unsigned char inb(unsigned short int port) {
    unsigned char ret;
    asm volatile("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
    return ret;
}

#define COM1 0x3f8

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

unsigned long long inituefi(void* image, efisystemtable_t* systab) {
    // Set up SSE
    __asm__ __volatile__ (
        "    movq %cr0, %rax\n"
        "    andb $0xF1, %al\n"
        "    movq %rax, %cr0\n"
        "    movq %cr4, %rax\n"
        "    orw $3 << 9, %ax\n"
        "    mov %rax, %cr4\n"
    );

    unsigned long long status = 0;

    // Setup
    efiguid_t gopguid = GOPGUID;
    status = systab->bservices->locprot(&gopguid, (void*)0, (void**)&gop);
    if(!EFIERROR(status) && gop) {
        gop->setmode(gop, 0);
    } else {
        wstrcom1("Error: Could not initialize GOP, halting!\n");
        asm volatile("cli; hlt");
    }

    efiloadedimageprot_t* lip = (void*)0;
    efiguid_t lipguid = LIPGUID;
    systab->bservices->handleprot(image, &lipguid, (void**)&lip);

    efifilehandle_t* rootdir = (void*)0;
    efisfsprot_t* sfs = (void*)0;
    efiguid_t sfsguid = SFSGUID;
    status = systab->bservices->handleprot(lip->devhanle, &sfsguid, (void**)&sfs);
    if(!EFIERROR(status)) {
        sfs->openvolume(sfs, &rootdir);
    }
    if(!rootdir) {
        wstrscr("Failed to open file.");
        while(1) asm volatile("hlt");
    }

    efifilehandle_t* kernelfile = &filedata;
    rootdir->open(rootdir, &kernelfile, kernelfilename, EFIFILEMODEREAD, 0);

    efifileinfo_t kernelfileinfo;
    efiguid_t fileinfoguid = EFIFILEINFOGUID;
    unsigned long long fileinfosize = sizeof(kernelfileinfo);
    kernelfile->getinfo(kernelfile, &fileinfoguid, &fileinfosize, &kernelfileinfo);

    void* kerneldata = (void*)0;
    systab->bservices->allocatepool(efiloaderdata, kernelfileinfo.filesize, &kerneldata);
    kernelfile->read(kernelfile, &(kernelfileinfo.filesize), kerneldata);
    kernelfile->close(kernelfile);

    sysparam_t bootparams = {0};
    bootparams.framebufferinfo.hres = gop->mode->info->hres;
    bootparams.framebufferinfo.vres = gop->mode->info->vres;
    bootparams.framebufferinfo.pitch = gop->mode->info->pixperscanline;
    bootparams.framebufferinfo.size = gop->mode->fbsize;

    if((((unsigned long long)(gop->mode->fbbase)) % PAGE2M)) {
        wstrscr("Warning: Framebuffer pointer is not aligned on a 2MB boundary!\n");
    }

    elf64ehdr_t *elf = (elf64ehdr_t*)kerneldata;
    elf64phdr_t *phdr = (void*)0;

    if(!(elf->entry)) {
        wstrscr("Warning: No entry!\n");
    }

    void* entry = (void*)(elf->entry);

    if(!(entry)) {
        wstrscr("Warning: No entry!\n");
    }

    unsigned long long ptmarker = 0;

    pt[ptmarker++] = ((unsigned long long)&(pml4[0])) | (PAGERW | PAGEP);

    unsigned long long i;
    for(i = 0, phdr = (elf64phdr_t*)(kerneldata + elf->phoff); i < elf->phnum; i++, phdr = (elf64phdr_t*)(((unsigned char*)phdr) + elf->phentsize)) {
        if(phdr->ptype == PTLOAD) {
            unsigned long long sectionpages = phdr->memsize / PAGE4K;
            if(phdr->memsize % PAGE4K) sectionpages++;
            void* sectionptr = (void*)0;
            status = systab->bservices->allocatepages(efiallocany, efiloaderdata, sectionpages, &sectionptr);
            if(EFIERROR(status)) {
                wstrscr("Error when allocating pages!\n");
            }
            memcpy(sectionptr, kerneldata + phdr->offset, phdr->filesize);
            memset(sectionptr + phdr->filesize, 0, phdr->memsize - phdr->filesize);
            for(unsigned long long j = 0; j < sectionpages; j++) {
                pt[ptmarker++] = ((unsigned long long)(sectionptr + (j * PAGE4K))) | (PAGERW | PAGEP);
            }
        }
    }

    unsigned long long fbpages = (gop->mode->fbsize) / PAGE2M;
    if((gop->mode->fbsize) % PAGE2M) fbpages++;
    if(fbpages > 512) {
        wstrscr("Warning: Framebuffer pages are larger than a single page directory!\n");
    }
    for(i = 0; i < fbpages; i++) {
        pd2[i] = (((unsigned long long)(gop->mode->fbbase)) + (i * PAGE2M)) | (PAGEPS | PAGERW | PAGEP);
    }

    pd1[0] = ((unsigned long long)&(pt[0])) | (PAGERW | PAGEP);

    for(i = 0; i < 512; i++) {
        // Identity map 1GB so our bootloader keeps running 
        pd[i] = (i * PAGE2M) | (PAGEPS | PAGERW | PAGEP);
    }

    pdpt[0] = ((unsigned long long)&(pd[0])) | (PAGERW | PAGEP);
    pdpt[2] = ((unsigned long long)&(pd2[0])) | (PAGERW | PAGEP); // 2GB mark for framebuffer
    pdpt[3] = ((unsigned long long)&(pd1[0])) | (PAGERW | PAGEP); // 3GB mark for kernel
    pml4[0] = ((unsigned long long)&(pdpt[0])) | (PAGERW | PAGEP);

    // get initial memmap size
    efimemdesc_t* memmap = (void*)0;
    unsigned long long memmapsize = 0;
    unsigned long long mapkey = 0;
    unsigned long long descsize = 0;
    unsigned long long descversion = 0;
    unsigned long long ogsize = 0;
    systab->bservices->getmemorymap(&memmapsize, memmap, &mapkey, &descsize, &descversion);

    // Allocate memory for the memory map
    // Add space for an extra 2 entries
    memmapsize += descsize * 2;
    ogsize = memmapsize;
    systab->bservices->allocatepool(efiloaderdata, memmapsize, (void**)&memmap);
    systab->bservices->getmemorymap(&memmapsize, memmap, &mapkey, &descsize, &descversion);

    systab->bservices->exitbootservices(image, mapkey);

    asm volatile("cli; movq %0, %%cr3" : : "b"(&(pml4[0])));

    gop->mode->fbbase = (unsigned int*)0x80000000ULL;

    if(!(entry)) {
        wstrscr("Warning: No entry!\n");
    }

    (*((void(* __attribute__((sysv_abi)))(sysparam_t*))(entry)))(&bootparams);

    return 0;
}
