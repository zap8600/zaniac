#include "idt.h"
#include "disp.h"

//
extern void* _isr0();
extern void* _isr1();
extern void* _isr2();
extern void* _isr3();
extern void* _isr4();
extern void* _isr5();
extern void* _isr6();
extern void* _isr7();
extern void* _isr8();
extern void* _isr9();
extern void* _isr10();
extern void* _isr11();
extern void* _isr12();
extern void* _isr13();
extern void* _isr14();
extern void* _isr15();
extern void* _isr16();
extern void* _isr17();
extern void* _isr18();
extern void* _isr19();
extern void* _isr20();
extern void* _isr21();
extern void* _isr22();
extern void* _isr23();
extern void* _isr24();
extern void* _isr25();
extern void* _isr26();
extern void* _isr27();
extern void* _isr28();
extern void* _isr29();
extern void* _isr30();
extern void* _isr31();
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

typedef struct idtptr_t {
    unsigned short int limit;
    unsigned long long base;
} __attribute__((packed)) idtptr_t;

__attribute__((aligned(16)))
idtentry_t idt[256];

idtptr_t idtr;

void idtsetdesc(unsigned char vec, void* isr, unsigned char flags) {
    idtentry_t* desc = &(idt[vec]);
    desc->isrlow = (unsigned long long)isr & 0xffff;
    desc->kernelcs = 8;
    desc->ist = 0;
    desc->attr = flags;
    desc->isrmid = ((unsigned long long)isr >> 16) & 0xffff;
    desc->isrhigh = ((unsigned long long)isr >> 32) & 0xffffffff;
    desc->reserved = 0;
}

void initidt() {
    idtr.base = (unsigned long long)(&(idt[0]));
    idtr.limit = sizeof(idt) - 1;

    //
    idtsetdesc(0, _isr0, 0x8e);
    idtsetdesc(1, _isr1, 0x8e);
    idtsetdesc(2, _isr2, 0x8e);
    idtsetdesc(3, _isr3, 0x8e);
    idtsetdesc(4, _isr4, 0x8e);
    idtsetdesc(5, _isr5, 0x8e);
    idtsetdesc(6, _isr6, 0x8e);
    idtsetdesc(7, _isr7, 0x8e);
    idtsetdesc(8, _isr8, 0x8e);
    idtsetdesc(9, _isr9, 0x8e);
    idtsetdesc(10, _isr10, 0x8e);
    idtsetdesc(11, _isr11, 0x8e);
    idtsetdesc(12, _isr12, 0x8e);
    idtsetdesc(13, _isr13, 0x8e);
    idtsetdesc(14, _isr14, 0x8e);
    idtsetdesc(15, _isr15, 0x8e);
    idtsetdesc(16, _isr16, 0x8e);
    idtsetdesc(17, _isr17, 0x8e);
    idtsetdesc(18, _isr18, 0x8e);
    idtsetdesc(19, _isr19, 0x8e);
    idtsetdesc(20, _isr20, 0x8e);
    idtsetdesc(21, _isr21, 0x8e);
    idtsetdesc(22, _isr22, 0x8e);
    idtsetdesc(23, _isr23, 0x8e);
    idtsetdesc(24, _isr24, 0x8e);
    idtsetdesc(25, _isr25, 0x8e);
    idtsetdesc(26, _isr26, 0x8e);
    idtsetdesc(27, _isr27, 0x8e);
    idtsetdesc(28, _isr28, 0x8e);
    idtsetdesc(29, _isr29, 0x8e);
    idtsetdesc(30, _isr30, 0x8e);
    idtsetdesc(31, _isr31, 0x8e);
    //

    asm volatile("lidt %0" : : "m"(idtr));
    asm volatile("sti");
}

__attribute__((noreturn))
void exceptionhandler(void);
void exceptionhandler() {
    asm volatile("cli");
    asm volatile("hlt");
}