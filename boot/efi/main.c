// Font
#include <kernel/font/VGA8.h>

// Boot params and memory information
#include <kernel/sysparam.h>

// Arch
#include <boot/efi/arch/arch.h>

// EFI
#include <boot/efi/efi.h>

__attribute__((naked))
void bootstrap() {
    asm volatile(
        "    .section .text.head\n"
        "    .globl image_base\n"
        "image_base:\n"
        "    .ascii \"MZ\"\n"
        "    .skip 58\n"
        "    .4byte pe_header - image_base\n"
        "pe_header:\n"
        "    .ascii \"PE\"\n"
        "    .2byte 0\n"
        "coff_header:\n"
#if defined(__x86_64__)
        "    .2byte 0x8664\n" // x86_64
#elif defined(__riscv) && (__riscv_xlen == 64)
        "    .2byte 0x5064\n" // RISCV-V64
#else
#error Unknown arch!
#endif
        "    .2byte 4\n"
        "    .4byte 0\n"
        "    .4byte 0\n"
        "    .4byte 0\n"
        "    .2byte section_table - optional_header\n"
        "    .2byte 0x206\n"
        "optional_header:\n"
        "    .2byte 0x20b\n"
        "    .byte 0x02\n"
        "    .byte 0x14\n"
        "    .4byte _text_size - image_base\n"
        "    .4byte _alldata_size - image_base\n"
        "    .4byte 0\n"
        "    .4byte _start - image_base\n"
        "    .4byte _text - image_base\n"
        "extra_header_fields:\n"
        "    .8byte 0\n"
        "    .4byte 0x1000\n"
        "    .4byte 0x1000\n"
        "    .2byte 0\n"
        "    .2byte 0\n"
        "    .2byte 0\n"
        "    .2byte 0\n"
        "    .2byte 0\n"
        "    .2byte 0\n"
        "    .4byte 0\n"
        "    .4byte _image_end - image_base\n"
        "    .4byte _text - image_base\n"
        "    .4byte 0\n"
        "    .2byte 10\n"
        "    .2byte 0\n"
        "    .8byte 0\n"
        "    .8byte 0\n"
        "    .8byte 0\n"
        "    .8byte 0\n"
        "    .4byte 0\n"
        "    .4byte 0x10\n"
        "    .8byte 0\n"
        "    .8byte 0\n"
        "    .8byte 0\n"
        "    .8byte 0\n"
        "    .8byte 0\n"
        "    .4byte _reloc - image_base\n"
        "    .4byte _reloc_vsize - image_base\n"
        "    .8byte 0\n"
        "    .8byte 0\n"
        "    .8byte 0\n"
        "    .8byte 0\n"
        "    .8byte 0\n"
        "    .8byte 0\n"
        "    .8byte 0\n"
        "    .8byte 0\n"
        "    .8byte 0\n"
        "    .8byte 0\n"
        "section_table:\n"
        "    .ascii \".text\\0\\0\\0\"\n"
        "    .4byte _text_vsize - image_base\n"
        "    .4byte _text - image_base\n"
        "    .4byte _text_size - image_base\n"
        "    .4byte _text - image_base\n"
        "    .4byte 0\n"
        "    .4byte 0\n"
        "    .2byte 0\n"
        "    .2byte 0\n"
        "    .4byte 0x60000020\n"

        "    .ascii \".data\\0\\0\\0\"\n"
        "    .4byte _data_vsize - image_base\n"
        "    .4byte _data - image_base\n"
        "    .4byte _data_size - image_base\n"
        "    .4byte _data - image_base\n"
        "    .4byte 0\n"
        "    .4byte 0\n"
        "    .2byte 0\n"
        "    .2byte 0\n"
        "    .4byte 0xC0000040\n"

        "    .ascii \".reloc\\0\\0\"\n"
        "    .4byte _reloc_vsize - image_base\n"
        "    .4byte _reloc - image_base\n"
        "    .4byte _reloc_size - image_base\n"
        "    .4byte _reloc - image_base\n"
        "    .4byte 0\n"
        "    .4byte 0\n"
        "    .2byte 0\n"
        "    .2byte 0\n"
        "    .4byte 0x42000040\n"

        "    .ascii \".rodata\\0\"\n"
        "    .4byte _rodata_vsize - image_base\n"
        "    .4byte _rodata - image_base\n"
        "    .4byte _rodata_size - image_base\n"
        "    .4byte _rodata - image_base\n"
        "    .4byte 0\n"
        "    .4byte 0\n"
        "    .2byte 0\n"
        "    .2byte 0\n"
        "    .4byte 0x40000040\n"
        "    .text\n"
        "    .globl _start\n"
        "_start:\n"
#if defined(__x86_64__)
        "    movq %cr0, %rax\n"
        "    andb $0xf1, %al\n"
        "    movq %rax, %cr0\n"
        "    movq %cr4, %rax\n"
        "    orw $3 << 9, %ax\n"
        "    mov %rax, %cr4\n"

        "    subq $8, %rsp\n"
        "    pushq %rcx\n"
        "    pushq %rdx\n"
        "    lea image_base(%rip), %rdi\n"
        "    lea image_base(%rip), %rsi\n"
        "    popq %rcx\n"
        "    popq %rdx\n"
        "    pushq %rcx\n"
        "    pushq %rdx\n"
        "    call _relocate\n"
        "    test %rax, %rax\n"
        "    jnz .exiterr\n"
        "    popq %rdi\n"
        "    popq %rsi\n"
        "    call inituefi\n"
        "    addq $8, %rsp\n"
        "    jmp .exit\n"
        ".exiterr:\n"
        "    addq $24, %rsp\n"
        ".exit:\n"
        "    ret\n"
#elif defined(__riscv) && (__riscv_xlen == 64)
        "    addi sp, sp, -24\n"
        "    sd a0, 0(sp)\n"
        "    sd a1, 8(sp)\n"
        "    sd ra, 16(sp)\n"
        "    lla a0, image_base\n"
        "    lla a1, _DYNAMIC\n"
        "    call _relocate\n"
        "    bne a0, zero, .exit\n"
        "    ld a1, 8(sp)\n"
        "    ld a0, 0(sp)\n"
        "    call inituefi\n"
        ".exit:\n"
        "    ld ra, 16(sp)\n"
        "    addi sp, sp, 24\n"
        "    ret\n"
#else
#error Unknown arch!
#endif
        "    .data\n"
        "dummy0: .4byte 0\n"
        "dummy1: .4byte 0\n"
        "    .section .reloc, \"a\"\n"
        "    .4byte dummy1 - dummy0\n"
        "    .4byte 12\n"
        "    .4byte 0\n"
        ".text\n"
    );
}

typedef struct elf64dyn_t {
    signed long long dtag;
    union {
        unsigned long long dval;
        unsigned long long dptr;
    } dun;
} elf64dyn_t;

typedef struct elf64rel_t {
    unsigned long long roffset;
    unsigned long long rinfo;
    signed long long raddend;
} elf64rel_t;

#if defined(__x86_64__)
unsigned long long _relocate(unsigned long long ldbase, elf64dyn_t* dyn, unsigned long long image __attribute__((unused)), efisystemtable_t* systab __attribute__((unused)))
#elif defined(__riscv) && (__riscv_xlen == 64)
unsigned long long _relocate(unsigned long long ldbase, elf64dyn_t* dyn)
#else
#error Unknown arch!
#endif
{
    long long relsz = 0;
    long long relent = 0;
    elf64rel_t* rel = (void*)0;
    unsigned long long* addr;
    for(int i = 0; dyn[i].dtag != 0; i++) {
        switch(dyn[i].dtag) {
            case 7: {
                rel = (elf64rel_t*)(dyn[i].dun.dptr + ldbase);
                break;
            }
            case 8: {
                relsz = dyn[i].dun.dval;
                break;
            }
            case 9: {
                relent = dyn[i].dun.dval;
                break;
            }
            default: break;
        }
    }
    if(!rel && (relent == 0)) {
        return 0;
    } else if(!rel || (relent == 0)) {
        return EFIERR(1); // Load error
    } else {
        while(relsz > 0) {
            if(((rel->rinfo) & 0xffffffff) == 3) {
                //arch_serial_send('H');
                //arch_serial_send(' ');
                addr = (unsigned long long*)(ldbase + rel->roffset);
                *addr += ldbase + rel->raddend;
            }
            rel = (elf64rel_t*)(((char*)rel) + relent); // The (char*) lets the compiler know that this may not be aligned, so it needs to not cause an unaligned load fault
            relsz -= relent;
        }
    }

    return 0;
}

// https://www.youtube.com/watch?v=mHh2-ixF9Yk&t=1031s

//Screen
efigop_t* gop = (void*)1;

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

void (*write_ch)(char ch) = (void*)0;

void wstr(const char* s) {
    char c = 0;
    while((c = *s++)) {
        write_ch(c);
    }
}

#define PAGE4K (4 * 1024)
#define PAGE2M (2 * 1024 * 1024)
#define PAGE1G (1 * 1024 * 1024 * 1024)

unsigned long long inituefi(void* image, efisystemtable_t* systab) {
    unsigned long long status = 0;

    sysparam_t bootparams = {0};

    // while(1) {
    //     arch_serial_send('H');
    // }

    // Setup
    efiguid_t gopguid = GOPGUID;
    status = systab->bservices->locprot(&gopguid, (void*)0, (void**)&gop);
    if(!EFIERROR(status) && gop) {
        gop->setmode(gop, 0);
        write_ch = wchscr;
        bootparams.framebufferinfo.present = 1;
        clearscr();
    } else {
        //arch_serial_send('H');
        write_ch = arch_serial_send; // wchcom1;
        bootparams.framebufferinfo.present = 0;
        wstr("Warning: GOP could not be initialized! Resulted in: ");
        wstr(ptrtostr(status));
        wstr("!\n");
        // asm volatile("cli; hlt");
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
        wstr("Error: Failed to open zaniac.elf!\n");
        while(1) arch_halt(); // asm volatile("cli; hlt");
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

    unsigned long long i;
    if(bootparams.framebufferinfo.present) {
        bootparams.framebufferinfo.hres = gop->mode->info->hres;
        bootparams.framebufferinfo.vres = gop->mode->info->vres;
        bootparams.framebufferinfo.pitch = gop->mode->info->pixperscanline;
        bootparams.framebufferinfo.size = gop->mode->fbsize;

        if((((unsigned long long)(gop->mode->fbbase)) % PAGE2M)) {
            wstr("Warning: Framebuffer pointer is not aligned on a 2MB boundary!\n");
        }

        unsigned long long fbpages = (gop->mode->fbsize) / PAGE2M;
        if((gop->mode->fbsize) % PAGE2M) fbpages++;
        if(fbpages > 512) {
            wstr("Warning: Framebuffer pages are larger than a single page directory!\n");
        }
        for(i = 0; i < fbpages; i++) {
            arch_framebuffer_map_2m((((unsigned long long)(gop->mode->fbbase)) + (i * PAGE2M)), i); // pd2[i] = (((unsigned long long)(gop->mode->fbbase)) + (i * PAGE2M)) | (PAGEPS | PAGERW | PAGEP);
        }
    }

    elf64ehdr_t *elf = (elf64ehdr_t*)kerneldata;
    elf64phdr_t *phdr = (void*)0;

    if(!(elf->entry)) {
        wstr("Warning: No entry!\n");
    }

    void* entry = (void*)(elf->entry);

    if(!(entry)) {
        wstr("Warning: No entry!\n");
    }

    unsigned long long ptmarker = 0;

    for(i = 0, phdr = (elf64phdr_t*)(kerneldata + elf->phoff); i < elf->phnum; i++, phdr = (elf64phdr_t*)(((unsigned char*)phdr) + elf->phentsize)) {
        if(phdr->ptype == PTLOAD) {
            unsigned long long sectionpages = phdr->memsize / PAGE4K;
            if(phdr->memsize % PAGE4K) sectionpages++;
            void* sectionptr = (void*)0;
            status = systab->bservices->allocatepages(efiallocany, efiloaderdata, sectionpages, &sectionptr);
            if(EFIERROR(status)) {
                wstr("Error when allocating pages!\n");
            }
            memcpy(sectionptr, kerneldata + phdr->offset, phdr->filesize);
            memset(sectionptr + phdr->filesize, 0, phdr->memsize - phdr->filesize);
            for(unsigned long long j = 0; j < sectionpages; j++) {
                arch_kernel_map_4k(((unsigned long long)(sectionptr + (j * PAGE4K))), ptmarker++); // pt[ptmarker++] = ((unsigned long long)(sectionptr + (j * PAGE4K))) | (PAGERW | PAGEP);
            }
        }
    }

    // pd1[0] = ((unsigned long long)&(pt[0])) | (PAGERW | PAGEP);

    for(i = 0; i < 512; i++) {
        // Identity map 1GB so our bootloader keeps running 
        arch_bootloader_map_2m((i * PAGE2M), i); // pd[i] = (i * PAGE2M) | (PAGEPS | PAGERW | PAGEP);
    }

    // pdpt[0] = ((unsigned long long)&(pd[0])) | (PAGERW | PAGEP); // 0GB - 1GB mark for bootloader identity map
    // pdpt[2] = ((unsigned long long)&(pd2[0])) | (PAGERW | PAGEP); // 2GB mark for framebuffer
    // pdpt[3] = ((unsigned long long)&(pd1[0])) | (PAGERW | PAGEP); // 3GB mark for kernel
    // pml4[0] = ((unsigned long long)&(pdpt[0])) | (PAGERW | PAGEP);

    // get the initial memory map size
    efimemdesc_t* memmap = (void*)0;
    unsigned long long memmapsize = 0;
    unsigned long long mapkey = 0;
    unsigned long long descsize = 0;
    unsigned int descversion = 0;
    systab->bservices->getmemorymap(&memmapsize, memmap, &mapkey, &descsize, &descversion);

    // Allocate memory for the memory map
    // Add space for an extra 2 entries
    memmapsize += descsize * 2;
    systab->bservices->allocatepool(efiloaderdata, memmapsize, (void**)&memmap);
    systab->bservices->getmemorymap(&memmapsize, memmap, &mapkey, &descsize, &descversion);

    // unsigned long long entries = memmapsize / descsize;

    systab->bservices->exitbootservices(image, mapkey);
    arch_init(); // asm volatile("cli; movq %0, %%cr3" : : "b"(&(pml4[0])));

    // unsigned char firstfound = 0;
    // memmarker_t* previous_marker = (void*)0;
    // memmarker_t* current_marker = (void*)0;

    // // Sort the memory map in order of physical address
    // sortmemmap(memmap, entries);
    // // Then we start putting in our memory markers
    // for(i = 0; i < entries; i++) {
    //     if(memmap[i].physicalstart >= PAGE1G) {
    //         wstrcom1("Warning: Exceeded page boundary with address ");
    //         wstrcom1(ptrtostr(memmap[i].physicalstart));
    //         wstrcom1(" of type ");
    //         wstrcom1(ptrtostr((unsigned long long)(memmap[i].type)));
    //         wstrcom1(" on index ");
    //         wstrcom1(ptrtostr(i));
    //         wstrcom1("!\n");
    //     }
    //     switch(memmap[i].type) {
    //         case efireservedmem:
    //         case efiloaderdata:
    //         case efirtscode:
    //         case efirtsdata:
    //         case efiunusablemem:
    //         case efiacpireclaimmem:
    //         case efiacpimemnvs:
    //         case efimemmappedio:
    //         case efimemmappedioport:
    //         case efipalcode:
    //         case efipersistmem:
    //         case efiunacceptedmem:
    //         {
    //             break;
    //         }

    //         case efibscode:
    //         case efibsdata:
    //         case eficonvetmem:
    //         case efiloadercode:
    //         default:
    //         {
    //             unsigned long long regionsize = (i != (entries - 1))?(memmap[i + 1].physicalstart - memmap[i].physicalstart):((memmap[i].physicalstart + (memmap[i].numofpages * PAGE4K)) - memmap[i].physicalstart);
    //             if(regionsize <= sizeof(memmarker_t)) break;
    //             if(!firstfound) {
    //                 current_marker = (memmarker_t*)(memmap[i].physicalstart);
    //                 current_marker->prev_free_addr = current_marker;
    //                 current_marker->size = regionsize - sizeof(memmarker_t);
    //                 current_marker->last = 0;
    //                 bootparams.memory_map = current_marker;
    //                 firstfound = 1;
    //             } else {
    //                 current_marker = (memmarker_t*)(memmap[i].physicalstart);
    //                 current_marker->prev_free_addr = previous_marker;
    //                 current_marker->size = regionsize - sizeof(memmarker_t);
    //                 current_marker->last = 1;
    //                 previous_marker->next_free_addr = current_marker;
    //             }
    //             previous_marker = current_marker;
    //             break;
    //         }
    //     }
    // }

    gop->mode->fbbase = (unsigned int*)0x80000000ULL;

    if(!(entry)) {
        wstr("Warning: No entry!\n");
    }

    (*((void(* __attribute__((sysv_abi)))(sysparam_t*))(entry)))(&bootparams);

    return 0;
}

#if defined(__x86_64__)
void outb(unsigned short int port, unsigned char value) {
    asm volatile("outb %b0, %w1" : : "a"(value), "Nd"(port) : "memory");
}

unsigned char inb(unsigned short int port) {
    unsigned char ret;
    asm volatile("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
    return ret;
}

#define COM1 0x3f8

void arch_serial_send(const char ch) {
    while(!(inb(COM1 + 5) & 0x20)) {}
    outb(COM1, ch);
}

#define PAGE4K (4 * 1024)
#define PAGE2M (2 * 1024 * 1024)
#define PAGE1G (1 * 1024 * 1024 * 1024)

#define PAGEPS 0x80 // PS flag
#define PAGERW 0x2 // Read/Write flag
#define PAGEP 0x1 // Present flag

unsigned long long pml4[512] __attribute__((aligned(PAGE4K))) = {0}; // root
unsigned long long pdpt[512] __attribute__((aligned(PAGE4K))) = {0}; // root
unsigned long long pd[512] __attribute__((aligned(PAGE4K))) = {0}; // for memmap
unsigned long long pd1[512] __attribute__((aligned(PAGE4K))) = {0}; // for kernel
unsigned long long pd2[512] __attribute__((aligned(PAGE4K))) = {0}; // for framebuffer
unsigned long long pt[512] __attribute__((aligned(PAGE4K))) = {0}; // for kernel

void arch_kernel_map_4k(unsigned long long physaddr, unsigned long long index) {
    pt[index] = physaddr | (PAGERW | PAGEP);
}

void arch_bootloader_map_2m(unsigned long long physaddr, unsigned long long index) {
    pd[index] = physaddr | (PAGEPS | PAGERW | PAGEP);
}

void arch_framebuffer_map_2m(unsigned long long physaddr, unsigned long long index) {
    pd2[index] = physaddr | (PAGEPS | PAGERW | PAGEP);
}

void arch_init() {
    pdpt[0] = ((unsigned long long)&(pd[0])) | (PAGERW | PAGEP); // 0GB - 1GB mark for bootloader identity map
    pdpt[2] = ((unsigned long long)&(pd2[0])) | (PAGERW | PAGEP); // 2GB mark for framebuffer
    pdpt[3] = ((unsigned long long)&(pd1[0])) | (PAGERW | PAGEP); // 3GB mark for kernel
    pml4[0] = ((unsigned long long)&(pdpt[0])) | (PAGERW | PAGEP);

    pd1[0] = ((unsigned long long)&(pt[0])) | (PAGERW | PAGEP);

    asm volatile("cli; movq %0, %%cr3" : : "b"(&(pml4[0])));
}

void arch_halt() {
    asm volatile("cli; hlt;");
}
#elif defined(__riscv) && (__riscv_xlen == 64)
void arch_serial_send(const char ch) {
    *((unsigned char*)0x10000000) = (unsigned char)ch;
}

#define PAGE4K (4 * 1024)
#define PAGE2M (2 * 1024 * 1024)
#define PAGE1G (1 * 1024 * 1024 * 1024)

#define PAGEV 0x1 // Valid flag
#define PAGER 0x2 // Read flag
#define PAGEW 0x4 // Write flag
#define PAGEX 0x8 // Execute flag

unsigned long long pdpt[512] __attribute__((aligned(PAGE4K))) = {0}; // root
unsigned long long pd[512] __attribute__((aligned(PAGE4K))) = {0}; // for memmap
unsigned long long pd1[512] __attribute__((aligned(PAGE4K))) = {0}; // for kernel
unsigned long long pd2[512] __attribute__((aligned(PAGE4K))) = {0}; // for framebuffer
unsigned long long pt[512] __attribute__((aligned(PAGE4K))) = {0}; // for kernel

void arch_kernel_map_4k(unsigned long long physaddr, unsigned long long index) {
    pt[index] = physaddr | (PAGEX | PAGEW | PAGER | PAGEV);
}

void arch_bootloader_map_2m(unsigned long long physaddr, unsigned long long index) {
    pd[index] = physaddr | (PAGEX | PAGEW | PAGER | PAGEV);
}

void arch_framebuffer_map_2m(unsigned long long physaddr, unsigned long long index) {
    pd2[index] = physaddr | (PAGEX | PAGEW | PAGER | PAGEV);
}

void arch_init() {
    pdpt[0] = (unsigned long long)&(pd[0]);
    pdpt[2] = (unsigned long long)&(pd2[0]);
    pdpt[3] = (unsigned long long)&(pd1[0]);

    pd[1] = (unsigned long long)&(pt[0]);

    wstr("Wish us luck!\n");

    // asm volatile("sfence.vma zero, zero");

    // TODO: Disable interrupts

    asm volatile("1: j 1b");

    asm volatile("csrw satp, %0" : : "r"(((8LL << 60) | (((unsigned long long)&(pdpt[0])) >> 12))));

    // asm volatile("sfence.vma zero, zero");

    wstr("We are still alive!\n");
}

void arch_halt() {
    // TODO: Disable interrupts

    asm volatile("wfi");
}
#else
#error Unknown arch!
#endif
