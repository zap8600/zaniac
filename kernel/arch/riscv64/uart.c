#include <kernel/arch/serial.h>

void arch_serial_init() {
    // Nothing to do right now
    return;
}

void arch_serial_send(char ch) {
    *((unsigned char*)0x10000000) = (unsigned char)ch; // TODO: Make this support more than the QEMU virt board
}