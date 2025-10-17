// Terminal abstraction code

#include <kernel/tty.h>
#include <kernel/framebuffer.h>
#include <kernel/arch/serial.h>

#include <kernel/font/VGA8.h>

#include <kernel/string.h>

static void fb_write_ch(char ch);
static framebufferinfo_t* framebufferinfo = (void*)0;

void (*write_ch)(char ch) = (void*)0;

void tty_init() {
    framebufferinfo = framebuffer_get_info();
    if(framebufferinfo->present) {
        write_ch = fb_write_ch;
        framebuffer_clear();
    } else {
        arch_serial_init();
        write_ch = arch_serial_send;
    }
}

void tty_write_ch(char ch) {
    write_ch(ch);
}

void tty_write_str(const char* str) {
    char ch = 0;
    while((ch = *str++)) {
        write_ch(ch);
    }
}

static unsigned int cy = 0;
static unsigned int cx = 0;

static void fb_scroll() {
    unsigned int* framebuffer = framebuffer_get_address();
    memcpy(framebuffer, framebuffer + (16 * framebufferinfo->pitch), (framebufferinfo->vres - 16) * framebufferinfo->pitch);
    memset(framebuffer + ((framebufferinfo->vres - 16) * framebufferinfo->pitch), 0, 16 * framebufferinfo->pitch);
}

static void fb_write_ch(char ch) {
    if(ch == '\n') {
        cx = 0;
        if((cy + 16) >= framebufferinfo->vres) {
            fb_scroll();
        } else {
            cy += 16;
        }
    } else {
        if((framebufferinfo->hres - cx) < 8) {
            cx = 0;
            if((cy + 16) >= framebufferinfo->vres) {
                fb_scroll();
            } else {
                cy += 16;
            }
        }
        for(unsigned int cb = 0; cb < 16; cb++) {
            const unsigned char c = VGA8_F16[(ch * 16) + cb];
            for(unsigned char shift = 0; shift < 8; shift++) {
                if(c & (0x80 >> shift)) {
                    framebuffer_draw_pixel(cx + shift, cy + cb, 0xffffffff);
                } else {
                    framebuffer_draw_pixel(cx + shift, cy + cb, 0x00000000);
                }
            }
        }
        cx += 8;
    }
}