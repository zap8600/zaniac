#ifndef _KERNEL_ARCH_ARCH_H
#define _KERNEL_ARCH_ARCH_H 1

#include <kernel/sysparam.h>

// Initialize architecture-specific data
void arch_init(sysparam_t* params);

#endif