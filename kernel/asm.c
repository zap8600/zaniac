#include "asm.h"

void cpuid(int code, unsigned int* a, unsigned int* b, unsigned int* c, unsigned int* d) {
    asm volatile("cpuid" : "=a"(*a), "=b"(*b), "=c"(*c), "=d"(*d) : "0"(code) :);
}