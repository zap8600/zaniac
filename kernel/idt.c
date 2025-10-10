#include "idt.h"
#include "disp.h"
#include "pic.h"

struct regs_t {
    // Automatically pushed by the interrupt
    unsigned long long rip;
    unsigned long long cs;
    unsigned long long rflags;
    unsigned long long rsp;
    unsigned long long ss;
};

#define ISRNOERR(index) \
__attribute__((naked)) \
void isr ## index () { \
    asm volatile("iretq"); \
}

//
ISRNOERR(0)
ISRNOERR(1)
ISRNOERR(2)
ISRNOERR(3)
ISRNOERR(4)
ISRNOERR(5)
ISRNOERR(6)
ISRNOERR(7)
ISRNOERR(8)
ISRNOERR(9)
ISRNOERR(10)
ISRNOERR(11)
ISRNOERR(12)
ISRNOERR(13)
ISRNOERR(14)
ISRNOERR(15)
ISRNOERR(16)
ISRNOERR(17)
ISRNOERR(18)
ISRNOERR(19)
ISRNOERR(20)
ISRNOERR(21)
ISRNOERR(22)
ISRNOERR(23)
ISRNOERR(24)
ISRNOERR(25)
ISRNOERR(26)
ISRNOERR(27)
ISRNOERR(28)
ISRNOERR(29)
ISRNOERR(30)
ISRNOERR(31)
//

typedef struct idtentry_t {
    unsigned short int isrlow;
    unsigned short int kernelcs;
    unsigned char ist;
    unsigned char attr;
    unsigned short int isrmid;
    unsigned int isrhigh;
    unsigned int reserved;
} __attribute__((packed)) idtentry_t;

struct idtptr_t {
    unsigned short int limit;
    unsigned long long base;
} __attribute__((packed));

__attribute__((aligned(16)))
static idtentry_t idt[256];

static struct idtptr_t idtr;

void idtsetdesc(unsigned char vec, void* isr, unsigned char flags) {
    unsigned long long base = (unsigned long long)isr;
    idt[vec].isrlow = base & 0xffff;
    idt[vec].kernelcs = 8;
    idt[vec].ist = 0;
    idt[vec].attr = flags;
    idt[vec].isrmid = (base >> 16) & 0xffff;
    idt[vec].isrhigh = (base >> 32) & 0xffffffff;
    idt[vec].reserved = 0;
}

void initidt() {
    idtr.base = (unsigned long long)&idt;
    idtr.limit = sizeof(idt) - 1;

    //
    idtsetdesc(0, isr0, 0x8e);
    idtsetdesc(1, isr1, 0x8e);
    idtsetdesc(2, isr2, 0x8e);
    idtsetdesc(3, isr3, 0x8e);
    idtsetdesc(4, isr4, 0x8e);
    idtsetdesc(5, isr5, 0x8e);
    idtsetdesc(6, isr6, 0x8e);
    idtsetdesc(7, isr7, 0x8e);
    idtsetdesc(8, isr8, 0x8e);
    idtsetdesc(9, isr9, 0x8e);
    idtsetdesc(10, isr10, 0x8e);
    idtsetdesc(11, isr11, 0x8e);
    idtsetdesc(12, isr12, 0x8e);
    idtsetdesc(13, isr13, 0x8e);
    idtsetdesc(14, isr14, 0x8e);
    idtsetdesc(15, isr15, 0x8e);
    idtsetdesc(16, isr16, 0x8e);
    idtsetdesc(17, isr17, 0x8e);
    idtsetdesc(18, isr18, 0x8e);
    idtsetdesc(19, isr19, 0x8e);
    idtsetdesc(20, isr20, 0x8e);
    idtsetdesc(21, isr21, 0x8e);
    idtsetdesc(22, isr22, 0x8e);
    idtsetdesc(23, isr23, 0x8e);
    idtsetdesc(24, isr24, 0x8e);
    idtsetdesc(25, isr25, 0x8e);
    idtsetdesc(26, isr26, 0x8e);
    idtsetdesc(27, isr27, 0x8e);
    idtsetdesc(28, isr28, 0x8e);
    idtsetdesc(29, isr29, 0x8e);
    idtsetdesc(30, isr30, 0x8e);
    idtsetdesc(31, isr31, 0x8e);
    //

    //remappic(32, 40);

    //

    asm volatile("lidt %0" : : "m"(idtr));
    asm volatile("sti");
}