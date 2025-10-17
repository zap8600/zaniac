#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

typedef struct framebufferinfo_t {
    unsigned int hres; // The horizontal resolution of the framebuffer
    unsigned int vres; // The vertical resolution of the framebuffer
    unsigned int pitch; // The pixels per scanline in the framebuffer
    unsigned long long size; // The size of the framebuffer
} framebufferinfo_t;

#endif