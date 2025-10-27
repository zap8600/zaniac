// RISC-V64 test

#include "VGA8.h"
#include "efi.h"

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

void wstr(const char* s) {
    char c = 0;
    while((c = *s++)) {
        wchscr(c);
    }
}

__attribute__((naked))
void bootstrap(void) {
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
        "    .2byte 0x5064\n"
        "    .2byte 4\n"
        "    .4byte 0\n"
        "    .4byte 0\n"
        "    .4byte 0\n"
        "    .2byte section_table - optional_header\n"
        "    .2byte 0x206\n"
        "optional_header:\n"
        "    .2byte 0x20b\n"
        "    .byte 0x2\n"
        "    .byte 0x14\n"
        "    .4byte _text_size - image_base\n"
        "    .4byte _alldata_size - image_base\n"
        "    .4byte 0\n"
        "    .4byte _start - image_base\n"
        "    .4byte _text - image_base\n"
        "extra_header_fields:\n"
        "    .8byte"

        ".align 4\n"
        "    .globl _start\n"
        "_start:\n"
        "    lla a2, image_base\n"
        "    lla a3, _DYNAMIC\n"
        "    call inituefi\n"
        "    ret\n"
        "    .data\n"
        "dummy: .long 0\n"
        "    .section .reloc, \"a\"\n"
        "label1:\n"
        "    .long dummy-label1\n"
        "    .long 10\n"
        "    .word 0\n"
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
} elf64rel_t;

unsigned long long inituefi(void* image, efisystemtable_t* systab, unsigned long long ldbase, elf64dyn_t* dyn) {
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
    if(rel && relent) {
        while(relsz > 0) {
            if(((rel->rinfo) & 0xffffffff) == 3) {
                addr = (unsigned long long*)(ldbase + rel->roffset);
                *addr += ldbase;
            }
            rel = (elf64rel_t*)(((char*)rel) + relent);
            relsz -= relent;
        }
    }

    unsigned long long status = 0;

    efiguid_t gopguid = GOPGUID;
    systab->bservices->locprot(&gopguid, (void*)0, (void**)&gop);

    wstr("Hello, RISC-V world!\n");
    
    while(1) {}

    return 1;
}