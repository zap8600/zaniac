#include <kernel/arch/x86_64/idt.h>
#include <kernel/arch/x86_64/pic.h>
#include <kernel/tty.h>

#include <stdint.h>

struct regs_t {
    // Automatically pushed by the interrupt
    uint64_t ip;
    uint64_t cs;
    uint64_t flags;
    uint64_t sp;
    uint64_t ss;
};

void isr_handler(uint8_t interruptnum, unsigned long code) __attribute__((no_caller_saved_registers));
void irq_handler(uint8_t interruptnum) __attribute__((no_caller_saved_registers));

#define ISRNOERR(index) \
__attribute__((interrupt)) \
void isr ## index(struct regs_t* r) { \
    isr_handler(index, 0); \
}

#define ISRERR(index) \
__attribute__((interrupt)) \
void isr ## index(struct regs_t* r, unsigned long code) { \
    isr_handler(index, code); \
}

#define IRQ(index) \
__attribute__((interrupt)) \
void irq ## index(struct regs_t* r) { \
    irq_handler(index); \
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
ISRERR(8)
ISRNOERR(9)
ISRERR(10)
ISRERR(11)
ISRERR(12)
ISRERR(13)
ISRERR(14)
ISRNOERR(15)
ISRNOERR(16)
ISRERR(17)
ISRNOERR(18)
ISRNOERR(19)
ISRNOERR(20)
ISRERR(21)
ISRNOERR(22)
ISRNOERR(23)
ISRNOERR(24)
ISRNOERR(25)
ISRNOERR(26)
ISRNOERR(27)
ISRNOERR(28)
ISRERR(29)
ISRERR(30)
ISRNOERR(31)
//

//
IRQ(0)
IRQ(1)
IRQ(2)
IRQ(3)
IRQ(4)
IRQ(5)
IRQ(6)
IRQ(7)
IRQ(8)
IRQ(9)
IRQ(10)
IRQ(11)
IRQ(12)
IRQ(13)
IRQ(14)
IRQ(15)
//

typedef struct idtentry_t {
    uint16_t isr_low;
    uint16_t kernel_cs;
    uint8_t ist;
    uint8_t attr;
    uint16_t isr_mid;
    uint32_t isr_high;
    uint32_t reserved;
} __attribute__((packed)) idtentry_t;

struct idtptr_t {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

__attribute__((aligned(16)))
static idtentry_t idt[256];

static struct idtptr_t idtr;

void idt_set_desc(uint8_t vec, void* isr, uint8_t flags) {
    uint64_t base = (uint64_t)isr;
    idt[vec].isr_low = base & 0xffff;
    idt[vec].kernel_cs = 8;
    idt[vec].ist = 0;
    idt[vec].attr = flags;
    idt[vec].isr_mid = (base >> 16) & 0xffff;
    idt[vec].isr_high = (base >> 32) & 0xffffffff;
    idt[vec].reserved = 0;
}

void idt_init() {
    idtr.base = (uint64_t)&idt;
    idtr.limit = sizeof(idt) - 1;

    //
    idt_set_desc(0, isr0, 0x8e);
    idt_set_desc(1, isr1, 0x8e);
    idt_set_desc(2, isr2, 0x8e);
    idt_set_desc(3, isr3, 0x8e);
    idt_set_desc(4, isr4, 0x8e);
    idt_set_desc(5, isr5, 0x8e);
    idt_set_desc(6, isr6, 0x8e);
    idt_set_desc(7, isr7, 0x8e);
    idt_set_desc(8, isr8, 0x8e);
    idt_set_desc(9, isr9, 0x8e);
    idt_set_desc(10, isr10, 0x8e);
    idt_set_desc(11, isr11, 0x8e);
    idt_set_desc(12, isr12, 0x8e);
    idt_set_desc(13, isr13, 0x8e);
    idt_set_desc(14, isr14, 0x8e);
    idt_set_desc(15, isr15, 0x8e);
    idt_set_desc(16, isr16, 0x8e);
    idt_set_desc(17, isr17, 0x8e);
    idt_set_desc(18, isr18, 0x8e);
    idt_set_desc(19, isr19, 0x8e);
    idt_set_desc(20, isr20, 0x8e);
    idt_set_desc(21, isr21, 0x8e);
    idt_set_desc(22, isr22, 0x8e);
    idt_set_desc(23, isr23, 0x8e);
    idt_set_desc(24, isr24, 0x8e);
    idt_set_desc(25, isr25, 0x8e);
    idt_set_desc(26, isr26, 0x8e);
    idt_set_desc(27, isr27, 0x8e);
    idt_set_desc(28, isr28, 0x8e);
    idt_set_desc(29, isr29, 0x8e);
    idt_set_desc(30, isr30, 0x8e);
    idt_set_desc(31, isr31, 0x8e);
    //

    pic_remap(32, 40);

    for(uint8_t i = 0; i < 16; i++) {
        if(!i) continue;
        pic_set_mask(i);
    }

    //
    idt_set_desc(32, irq0, 0x8e);
    idt_set_desc(33, irq1, 0x8e);
    idt_set_desc(34, irq2, 0x8e);
    idt_set_desc(35, irq3, 0x8e);
    idt_set_desc(36, irq4, 0x8e);
    idt_set_desc(37, irq5, 0x8e);
    idt_set_desc(38, irq6, 0x8e);
    idt_set_desc(39, irq7, 0x8e);
    idt_set_desc(40, irq8, 0x8e);
    idt_set_desc(41, irq9, 0x8e);
    idt_set_desc(42, irq10, 0x8e);
    idt_set_desc(43, irq11, 0x8e);
    idt_set_desc(44, irq12, 0x8e);
    idt_set_desc(45, irq13, 0x8e);
    idt_set_desc(46, irq14, 0x8e);
    idt_set_desc(47, irq15, 0x8e);
    //

    asm volatile("lidt %0" : : "m"(idtr));
    asm volatile("sti");
}

char *exception_messages[] = {
    "Division By Zero\n",
    "Debug\n",
    "Non-maskable Interrupt\n",
    "Breakpoint\n",
    "Overflow\n",
    "Bound Range Exceeded\n",
    "Invalid Opcode\n",
    "Device Not Available\n",
    "Double Fault\n",
    "Coprocessor Segment Overrun\n",
    "Invalid TSS\n",
    "Segment Not Present\n",
    "Stack-Segment Fault\n",
    "General Protection Fault\n",
    "Page Fault\n",
    "Reserved\n",
    "x87 Floating-Point Exception\n",
    "Alignment Check\n",
    "Machine Check\n",
    "SIMD Floating-Point Exception\n",
    "Virtualization\n",
    "Control Protection Exception\n",
    "Reserved\n",
    "Reserved\n",
    "Reserved\n",
    "Reserved\n",
    "Reserved\n",
    "Reserved\n",
    "Hypervisor Injection Exception\n",
    "VMM Communication Exception\n",
    "Security Exception\n",
    "Reserved\n"
};

char *irq_messages[] = {
    "PIT\n",
    "Keyboard\n",
    "Cascade?\n",
    "COM2\n",
    "COM1\n",
    "LPT2\n",
    "Floppy Disk\n",
    "LPT1\n",
    "CMOS\n",
    "Free\n",
    "Free\n",
    "Free\n",
    "PS2 Mouse\n",
    "FPU\n",
    "Prime ATA\n",
    "Secondary ATA\n"
};

void isr_handler(uint8_t interruptnum, unsigned long code) {
    tty_write_str(exception_messages[interruptnum]);
    if(interruptnum != 3) {
        asm volatile("1: jmp 1b");
    }
}

void irq_handler(uint8_t interruptnum) {
    tty_write_str(irq_messages[interruptnum]);
    if(!interruptnum) ;

    pic_acknowledge(interruptnum);
}
