#ifndef KERNEL_FRAMEBUFFER_H
#define KERNEL_FRAMEBUFFER_H 1

typedef struct framebufferinfo_t {
    unsigned char present; // Tells us if we actually have a framebuffer
    unsigned int hres; // The horizontal resolution of the framebuffer
    unsigned int vres; // The vertical resolution of the framebuffer
    unsigned int pitch; // The pixels per scanline in the framebuffer
    unsigned long long size; // The size of the framebuffer
} framebufferinfo_t;

// Initialize the framebuffer
void framebuffer_init(framebufferinfo_t* newframebufferinfo);

// Get the address of the framebuffer. Should be used for manual operations.
void* framebuffer_get_address();

// Get the framebuffer info
framebufferinfo_t* framebuffer_get_info();

// Draw a pixel in the framebuffer
void framebuffer_draw_pixel(unsigned int x, unsigned int y, unsigned int color);

// Clear the framebuffer
void framebuffer_clear();

#endif