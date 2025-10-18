#include <kernel/arch/x86_64/gdt.h>

#include <stdint.h>

typedef struct gdtentry_t {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed)) gdtentry_t;

typedef struct gdtentryhigh_t {
    uint32_t base_highest;
    uint32_t reserved;
} __attribute__((packed)) gdtentryhigh_t;

typedef struct tssentry_t {
    uint32_t reserved0;
    uint64_t rsp[3];
    uint64_t reserved1;
    uint64_t ist[7];
    uint64_t reserved2;
    uint64_t reserved3;
    uint16_t iomap_base;
} __attribute__((packed)) tssentry_t;

typedef struct gdtptr_t {
    uint16_t limit;
    uint64_t base;
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

void gdt_init() {
    gdt.ptr.limit = (sizeof(gdt.entries) + sizeof(gdt.tssextra)) - 1;
    gdt.ptr.base = (uint64_t)(&(gdt.entries));
    uint64_t addr = (uint64_t)(&(gdt.tss));
    gdt.entries[5].limit_low = sizeof(gdt.tss);
    gdt.entries[5].base_low = (addr & 0xffff);
    gdt.entries[5].base_middle = (addr >> 16) & 0xff;
    gdt.entries[5].base_high = (addr >> 24) & 0xff;
    gdt.tssextra.base_highest = (addr >> 32) & 0xffffffff;
    extern void* stack_top;
    gdt.tss.rsp[0] = (uint64_t)(&stack_top);
    asm volatile(
        "lgdt %0\n"
        "pushq $0x08\n"
        "lea .set_cs(%%rip), %%rax\n"
        "pushq %%rax\n"
        "lretq\n"
        ".set_cs:\n"
        "mov $0x10, %%ax\n"
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"
        "mov %%ax, %%ss\n"
        : : "m"(gdt.ptr) : "rax", "memory"
    );
}