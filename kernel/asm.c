#include "asm.h"

// Inline ASM guide
// a: ax register
// b: bx register
// c: cx register
// d: dx register
// =: write-only
// 0: ???

void cpuid(int code, unsigned int* a, unsigned int* b, unsigned int* c, unsigned int* d) {
    asm volatile("cpuid" : "=a"(*a), "=b"(*b), "=c"(*c), "=d"(*d) : "0"(code) :);
}

void writemsr(unsigned long long id, unsigned long long value) {
    unsigned int low = value & 0xffffffff;
    unsigned int high = value >> 32;
    asm volatile("wrmsr" : : "c"(id), "a"(low), "d"(high));
}

unsigned long long readmsr(unsigned int id) {
    unsigned int low;
    unsigned int high;
    asm volatile("rdmsr" : "=a"(low), "=d"(high) : "c"(id));
    return((unsigned long long)high << 32) | low;
}

void outb(unsigned short int port, unsigned char value) {
    asm volatile("outb %b0, %w1" : : "a"(value), "Nd"(port) : "memory");
}

void inb(unsigned short int port) {
    unsigned char ret;
    asm volatile("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
}