#ifndef _KERNEL_ARCH_SERIAL_H
#define _KERNEL_ARCH_SERIAL_H 1

// Initialize the serial port for this architecture
void arch_serial_init();

// Send a character over the architecture's serial port
void arch_serial_send(char ch);

#endif