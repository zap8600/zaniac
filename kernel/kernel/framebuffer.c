// Simple framebuffer driver

#include <kernel/framebuffer.h>

// The bootloader should map our framebuffer to the 2GB mark
static void* framebuffer = (void*)0x80000000ULL;
static framebufferinfo_t framebufferinfo = {0};

void framebuffer_init(framebufferinfo_t* newframebufferinfo) {
    framebufferinfo = *newframebufferinfo;
    // TODO: Check if the framebuffer is present
    // TODO: Get type of framebuffer
}

void* framebuffer_get_address() {
    return framebuffer;
}

framebufferinfo_t* framebuffer_get_info() {
    return &framebufferinfo;
}

void framebuffer_draw_pixel(unsigned int x, unsigned int y, unsigned int color) {
    // Reverse the order of color so it gets written properly on little-endian architectures
    color = (color & 0xff) << 24 | (color & 0xff00) << 8 | (color & 0xff0000) >> 8 | (color & 0xff000000) >> 24;
    // TODO: Get type of framebuffer
    ((unsigned int*)(framebuffer))[(y * framebufferinfo.pitch) + x] = color;
}

void framebuffer_clear() {
    for(unsigned int y = 0; y < framebufferinfo.vres; y++) {
        for(unsigned int x = 0; x < framebufferinfo.hres; x++) {
            // TODO: Get type of framebuffer
            ((unsigned int*)(framebuffer))[(y * framebufferinfo.pitch) + x] = 0x00000000;
        }
    }
}