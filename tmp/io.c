#include "asm.h"
#include "io.h"

#define COM1 0x3f8

void wchcom1(const char c) {
    while(!(inb(COM1 + 5) & 0x20)) {}
    outb(COM1, c);
}

void wstrcom1(const char* s) {
    char c = 0;
    while((c = *s++)) {
        wchcom1(c);
    }
}