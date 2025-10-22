#ifndef _KERNEL_SYSPARAM_H
#define _KERNEL_SYSPARAM_H 1

#include "framebuffer.h"
#include "mem.h"

typedef struct sysparam_t {
    framebufferinfo_t framebufferinfo; // A structure containing information about the (possible) framebuffer given
    memmarker_t* memory_map; // A pointer to the memory map
} sysparam_t;

#endif