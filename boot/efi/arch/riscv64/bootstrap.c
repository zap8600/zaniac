#include <boot/efi/efi.h>
#include <boot/efi/arch/arch.h>

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
        "    .2byte 0x5064\n" // RISCV-V64
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

unsigned long long _relocate(unsigned long long ldbase, elf64dyn_t* dyn) {
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

    asm volatile("sfence.vma zero, zero");

    // TODO: Disable interrupts

    asm volatile("csrw satp, %0" : : "r"(((8LL << 60) | (((unsigned long long)&(pdpt[0])) >> 12))));

    asm volatile("sfence.vma zero, zero");
}

void arch_halt() {
    // TODO: Disable interrupts

    asm volatile("wfi");
}

