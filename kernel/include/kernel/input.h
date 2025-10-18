#ifndef _KERNEL_INPUT_H
#define _KERNEL_INPUT_H 1

#include <stddef.h>

extern char* input_buf;
extern size_t available_chs;

void input_init();
char input_get_ch();

#endif