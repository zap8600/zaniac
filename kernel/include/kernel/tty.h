#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H 1

#include <kernel/sysparam.h>

// Initialize the early console for debugging
void tty_early_init(sysparam_t* params);

// Write a character to the console
void tty_write_ch(char ch);

// Write a string to the console
void tty_write_str(const char* str);

#endif