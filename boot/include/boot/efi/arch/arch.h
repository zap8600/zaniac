#ifndef _BOOT_EFI_ARCH_ARCH_H
#define _BOOT_EFI_ARCH_ARCH_H 1

void arch_kernel_map_4k(unsigned long long physaddr, unsigned long long index);
void arch_bootloader_map_2m(unsigned long long physaddr, unsigned long long index);
void arch_framebuffer_map_2m(unsigned long long physaddr, unsigned long long index);
void arch_init();
void arch_halt();

void arch_serial_send(const char ch);

#endif