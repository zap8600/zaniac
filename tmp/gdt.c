#include "gdt.h"

typedef struct gdtentry_t {
    unsigned short int limitlow;
    unsigned short int baselow;
    unsigned char basemiddle;
    unsigned char access;
    unsigned char granularity;
    unsigned char basehigh;
} __attribute__((packed)) gdtentry_t;

typedef struct gdtentryhigh_t {
    unsigned int basehighest;
    unsigned int reserved;
} __attribute__((packed)) gdtentryhigh_t;

typedef struct tssentry_t {
    unsigned int reserved0;
    unsigned long long rsp[3];
    unsigned long long reserved1;
    unsigned long long ist[7];
    unsigned long long reserved2;
    unsigned long long reserved3;
    unsigned short int iomapbase;
} __attribute__((packed)) tssentry_t;

typedef struct gdtptr_t {
    unsigned short int limit;
    unsigned long long base;
} __attribute__((packed)) gdtptr_t;

typedef struct gdt_t {
    gdtentry_t entries[6];
    gdtentryhigh_t tssextra;
    gdtptr_t ptr;
    tssentry_t tss;
} __attribute__((packed)) __attribute__((aligned(16))) gdt_t;

gdt_t gdt __attribute__((used)) = {
    {
        {0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00},
        {0xffff, 0x0000, 0x00, 0x9a, 0xaf, 0x00},
        {0xffff, 0x0000, 0x00, 0x92, 0xcf, 0x00},
        {0xffff, 0x0000, 0x00, 0xfa, 0xaf, 0x00},
        {0xffff, 0x0000, 0x00, 0xf2, 0xca, 0x00},
        {0x0000, 0x0000, 0x00, 0x89, 0xc0, 0x00}
    },
    {0x00000000, 0x00000000},
    {0x0000, 0x0000000000000000},
    {0, {0, 0, 0}, 0, {0, 0, 0, 0, 0, 0, 0}, 0, 0, 0}
};

void initgdt() {
    gdt.ptr.limit = (sizeof(gdt.entries) + sizeof(gdt.tssextra)) - 1;
    gdt.ptr.base = (unsigned long long)(&(gdt.entries));
    unsigned long long addr = (unsigned long long)(&(gdt.tss));
    gdt.entries[5].limitlow = sizeof(gdt.tss);
    gdt.entries[5].baselow = (addr & 0xffff);
    gdt.entries[5].basemiddle = (addr >> 16) & 0xff;
    gdt.entries[5].basehigh = (addr >> 24) & 0xff;
    gdt.tssextra.basehighest = (addr >> 32) & 0xffffffff;
    extern void* stacktop;
    gdt.tss.rsp[0] = (unsigned long long)(&stacktop);
    asm volatile(
        "lgdt %0\n"
        "mov $16, %%ax\n"
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%ss\n"
        "mov $43, %%ax\n"
        "ltr %%ax\n"
        : : "m"(gdt.ptr) : "rax", "memory"
    );
}