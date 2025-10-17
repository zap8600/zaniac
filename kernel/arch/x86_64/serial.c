#include <kernel/arch/serial.h>
#include <kernel/arch/x86_64/asm.h>

#define COM1 0x3f8

void arch_serial_init() {
    // Nothing to do right now
    return;
}

void arch_serial_send(char ch) {
    while(!(inb(COM1 + 5) & 0x20)) {}
    outb(COM1, ch);
}