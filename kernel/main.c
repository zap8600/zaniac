#include "sysyparam.h"

void* memcpy(void* d, void* s, unsigned long int n) {
    unsigned char* dst = (unsigned char*)d;
    const unsigned char* src = (const unsigned char*)s;
    for(unsigned long int i = 0; i < n; i++) {
        dst[i] = src[i];
    }
    return d;
}

sysparam_t params;

void kernel_main(sysparam_t* bootparams) {
    memcpy(&params, bootparams, sizeof(sysparam_t));
    for(unsigned int y = 0; y < params.vres; y++) {
        for(unsigned int x = 0; x < params.hres; x++) {
            ((unsigned int*)(params.framebuffer))[(y * params.pitch) + x] = (((!(y % 2)) && (!(x % 2))) ? 0xffffffff : 0x00000000); // Creates a checkerboard pattern
        }
    }
    asm volatile("cli");
    asm volatile("hlt");
}