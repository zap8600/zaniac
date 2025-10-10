#include "idt.h"
#include "disp.h"

typedef struct regs_t {
    // Saves registers, we might need these
    unsigned long long r15;    
    unsigned long long r14;
    unsigned long long r13;
    unsigned long long r12;
    unsigned long long r11;
    unsigned long long r10;
    unsigned long long r9;
    unsigned long long r8;
    unsigned long long rbp;
    unsigned long long rdi;
    unsigned long long rsi;
    unsigned long long rdx;
    unsigned long long rcx;
    unsigned long long rbx;
    unsigned long long rax;

    // Automatically pushed by the interrupt
    unsigned long long rip;
    unsigned long long cs;
    unsigned long long rflags;
    unsigned long long rsp;
    unsigned long long ss;
} regs_t;

//
extern regs_t* _isr0(regs_t*);
extern regs_t* _isr1(regs_t*);
extern regs_t* _isr2(regs_t*);
extern regs_t* _isr3(regs_t*);
extern regs_t* _isr4(regs_t*);
extern regs_t* _isr5(regs_t*);
extern regs_t* _isr6(regs_t*);
extern regs_t* _isr7(regs_t*);
extern regs_t* _isr8(regs_t*);
extern regs_t* _isr9(regs_t*);
extern regs_t* _isr10(regs_t*);
extern regs_t* _isr11(regs_t*);
extern regs_t* _isr12(regs_t*);
extern regs_t* _isr13(regs_t*);
extern regs_t* _isr14(regs_t*);
extern regs_t* _isr15(regs_t*);
extern regs_t* _isr16(regs_t*);
extern regs_t* _isr17(regs_t*);
extern regs_t* _isr18(regs_t*);
extern regs_t* _isr19(regs_t*);
extern regs_t* _isr20(regs_t*);
extern regs_t* _isr21(regs_t*);
extern regs_t* _isr22(regs_t*);
extern regs_t* _isr23(regs_t*);
extern regs_t* _isr24(regs_t*);
extern regs_t* _isr25(regs_t*);
extern regs_t* _isr26(regs_t*);
extern regs_t* _isr27(regs_t*);
extern regs_t* _isr28(regs_t*);
extern regs_t* _isr29(regs_t*);
extern regs_t* _isr30(regs_t*);
extern regs_t* _isr31(regs_t*);
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
void exceptionhandler(regs_t* r);
void exceptionhandler(regs_t* r) {
    // Someone told me the hypervisor doesn't like me stopping all CPUs and disabling interrupts so
    // Maybe don't do this
    // asm volatile("cli");
    while(1) asm volatile("hlt");
}