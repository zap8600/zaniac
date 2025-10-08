#include "sysparam.h"

void* memcpy(void* d, void* s, unsigned long long n) {
    unsigned char* dst = (unsigned char*)d;
    const unsigned char* src = (const unsigned char*)s;
    for(unsigned long long i = 0; i < n; i++) {
        dst[i] = src[i];
    }
    return d;
}

sysparam_t params;

#define COM1 0x3f8

static inline void outb(unsigned short port, unsigned char val) {
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile ( "inb %w1, %b0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}

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

void kernel_main(sysparam_t* bootparams) {
    memcpy(&params, bootparams, sizeof(sysparam_t));
    wstrcom1("We made it!\n");
    for(unsigned int y = 0; y < params.vres; y++) {
        for(unsigned int x = 0; x < params.hres; x++) {
            ((unsigned int*)(params.framebuffer))[(y * params.pitch) + x] = (((!(y % 2)) && (!(x % 2))) ? 0xff00ffff : 0x00000000); // Creates a checkerboard pattern
        }
    }
    while(1) asm volatile("hlt");
}