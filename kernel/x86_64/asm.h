#ifndef ASM_H
#define ASM_H

void cpuid(int code, unsigned int* a, unsigned int* b, unsigned int* c, unsigned int* d);
void writemsr(unsigned long long id, unsigned long long value);
unsigned long long readmsr(unsigned int id);
void outb(unsigned short int port, unsigned char value);
void inb(unsigned short int port);

#endif