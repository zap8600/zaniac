#include "disp.h"
#include "string.h"
#include "../font/VGA8.h"
#include "framebuffer.h"

static const void* framebuffer = (void*)0x80000000ULL; // 2GB
static framebufferinfo_t framebufferinfo;

// (unsigned long long)((&(framebuffer[((vres - 1) * pitch) + (hres - 1)])) - (&(framebuffer[pitch])))

void clearscr() {
    for(unsigned int y = 0; y < framebufferinfo.vres; y++) {
        for(unsigned int x = 0; x < framebufferinfo.hres; x++) {
            ((unsigned int*)(framebuffer))[(y * framebufferinfo.pitch) + x] = 0x00000000;
        } 
    }
    //
    return;
}

unsigned int cx = 0;
unsigned int cy = 0;
void wchscr(const char c) {
    if(c == '\n') {
        cx = 0;
        if((cy + 16) >= framebufferinfo.vres) {
            memcpy(framebuffer, framebuffer + (16 * framebufferinfo.pitch), (framebufferinfo.vres - 16) * framebufferinfo.pitch);
            memset(framebuffer + ((framebufferinfo.vres - 16) * framebufferinfo.pitch), 0, 16 * framebufferinfo.pitch);
            cy = 0;
        } else {
            cy += 16;
        }
    } else {
        if((framebufferinfo.hres - cx) < 8) {
            cx = 0;
            if((cy + 16) >= framebufferinfo.vres) {
                memcpy(framebuffer, framebuffer + (16 * framebufferinfo.pitch), (framebufferinfo.vres - 16) * framebufferinfo.pitch);
                memset(framebuffer + ((framebufferinfo.vres - 16) * framebufferinfo.pitch), 0, 16 * framebufferinfo.pitch);
                cy = 0;
            } else {
                cy += 16;
            }
        }
        for(unsigned int cb = 0; cb < 16; cb++) {
            const unsigned char ch = VGA8_F16[(c * 16) + cb];
            for(unsigned char shift = 0; shift < 8; shift++) {
                if(ch & (0x80 >> shift)) {
                    ((unsigned int*)(framebuffer))[((cy + cb) * framebufferinfo.pitch) + (cx + shift)] = 0xffffffff;
                } else {
                    ((unsigned int*)(framebuffer))[((cy + cb) * framebufferinfo.pitch) + (cx + shift)] = 0x00000000;
                }
            }
        }
        cx += 8;
    }
}

void wstrscr(const char* s) {
    char c = 0;
    while((c = *s++)) {
        wchscr(c);
    }
}

void resetcon() {
    cx = 0;
    cy = 0;
}

void drawcheckerboardpattern(unsigned int color) {
    color = (color & 0xff) << 24 | (color & 0xff00) << 8 | (color & 0xff0000) >> 8 | (color & 0xff000000) >> 24;
    for(unsigned int y = 0; y < framebufferinfo.vres; y++) {
        for(unsigned int x = 0; x < framebufferinfo.hres; x++) {
            ((unsigned int*)(framebuffer))[(y * framebufferinfo.pitch) + x] = (((!(y % 2)) && (!(x % 2))) ? color : 0x00000000);
        } 
    }
}

void initdisp(unsigned int hr, unsigned int vr, unsigned int p) {
    framebufferinfo.hres = hr;
    framebufferinfo.vres = vr;
    framebufferinfo.pitch = p;
    clearscr();
}