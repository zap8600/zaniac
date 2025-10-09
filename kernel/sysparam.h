#ifndef SYSPARAM_H
#define SYSPARAM_H

typedef struct sysparam_t {
    void* framebuffer;
    unsigned int hres;
    unsigned int vres;
    unsigned int pitch;
} sysparam_t;

#endif