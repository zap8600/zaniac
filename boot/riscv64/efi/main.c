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
char* uart = (void*)0x10000000;
void wchscr(const char c) {
    *uart = c;
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
        "    .8byte 0\n"
        "    .4byte 0x1000\n"
        "    .4byte 0x1000\n"
        "    .2byte 0\n"
        "    .2byte 0\n"
        "    .2byte 0\n"
        "    .2byte 0\n"
        "    .2byte 0\n"
        "    .2byte 0\n"
        "    .2byte 0\n"
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
        "    lla a2, image_base\n"
        "    lla a3, _DYNAMIC\n"
        "    call inituefi\n"
        "    ret\n"
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
} elf64rel_t;

unsigned long long inituefi(void* image, efisystemtable_t* systab, unsigned long long ldbase, elf64dyn_t* dyn) {
    // long long relsz = 0;
    // long long relent = 0;
    // elf64rel_t* rel = (void*)0;
    // unsigned long long* addr;
    // for(int i = 0; dyn[i].dtag != 0; i++) {
    //     switch(dyn[i].dtag) {
    //         case 7: {
    //             rel = (elf64rel_t*)(dyn[i].dun.dptr + ldbase);
    //             break;
    //         }
    //         case 8: {
    //             relsz = dyn[i].dun.dval;
    //             break;
    //         }
    //         case 9: {
    //             relent = dyn[i].dun.dval;
    //             break;
    //         }
    //         default: break;
    //     }
    // }
    // if(rel && relent) {
    //     while(relsz > 0) {
    //         if(((rel->rinfo) & 0xffffffff) == 3) {
    //             addr = (unsigned long long*)(ldbase + rel->roffset);
    //             *addr += ldbase;
    //         }
    //         rel = (elf64rel_t*)(((char*)rel) + relent);
    //         relsz -= relent;
    //     }
    // }

    // unsigned long long status = 0;

    //wstr("Hello, RISC-V world!\n");
    
    // while(1) {}

    return 0;
}