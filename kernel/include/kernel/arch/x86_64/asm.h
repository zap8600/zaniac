#ifndef KERNEL_ARCH_X86_64_ASM_H
#define KERNEL_ARCH_X86_64_ASM_H 1

void cpuid(int code, unsigned int* a, unsigned int* b, unsigned int* c, unsigned int* d);
void writemsr(unsigned long long id, unsigned long long value);
unsigned long long readmsr(unsigned int id);
void outb(unsigned short int port, unsigned char value);
unsigned char inb(unsigned short int port);
void iowait();

#endif