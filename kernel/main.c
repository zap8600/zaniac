#include "sysparam.h"
#include "mem/page.h"

void* memcpy(void* d, void* s, unsigned long long n) {
    unsigned char* dst = (unsigned char*)d;
    const unsigned char* src = (const unsigned char*)s;
    for(unsigned long long i = 0; i < n; i++) {
        dst[i] = src[i];
    }
    return d;
}

sysparam_t params;

void draw_checkerboard_pattern(unsigned int color) {
    unsigned int formattedcolor = (color & 0xff) << 24 | (color & 0xff00) << 8 | (color & 0xff0000) >> 8 | (color & 0xff000000) >> 24;
    for(unsigned int y = 0; y < params.vres; y++) {
        for(unsigned int x = 0; x < params.hres; x++) {
            ((unsigned int*)(params.framebuffer))[(y + params.pitch) + x] = ((!(x % 2)) && (!(y % 2))) ? formattedcolor : 0x00000000;
        } 
    }
}

void kernel_main(sysparam_t* bootparams) {
    memcpy(&params, bootparams, sizeof(sysparam_t));
    draw_checkerboard_pattern(0xff00ffff);
    asm volatile("cli");
    while(1) asm volatile("hlt");
}