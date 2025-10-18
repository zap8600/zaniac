#include <kernel/arch/x86_64/pic.h>
#include <kernel/arch/x86_64/asm.h>

#define PIC1CMD 0x20
#define PIC2CMD 0xA0
#define PIC1DATA 0x21
#define PIC2DATA 0xA1

void pic_remap(int offset1, int offset2) {
    outb(PIC1CMD, 0x11);
    iowait();
    outb(PIC2CMD, 0x11);
    iowait();
    outb(PIC1DATA, offset1);
    iowait();
    outb(PIC2DATA, offset2);
    iowait();
    outb(PIC1DATA, 1 << 2);
    iowait();
    outb(PIC2DATA, 2);
    iowait();

    // Configure in 8086 mode
    outb(PIC1DATA, 0x1);
    outb(PIC2DATA, 0x1);

    // Unmask
    outb(PIC1DATA, 0);
    outb(PIC2DATA, 0);
}

void pic_set_mask(unsigned char irqline) {
    unsigned short int port;
    unsigned char value;
    if(irqline < 8) {
        port = PIC1DATA;
    } else {
        port = PIC2DATA;
        irqline -= 8;
    }
    value = inb(port) | (1 << irqline);
    outb(port, value);
}

void pic_clear_mask(unsigned char irqline) {
    unsigned short int port;
    unsigned char value;
    if(irqline < 8) {
        port = PIC1DATA;
    } else {
        port = PIC2DATA;
        irqline -= 8;
    }
    value = inb(port) & ~(1 << irqline);
    outb(port, value);
    iowait();
}

void pic_acknowledge(unsigned char irqline) {
    outb(PIC1CMD, 0x20);
    if(irqline >= 8) {
        outb(PIC2CMD, 0x20);
    }
}