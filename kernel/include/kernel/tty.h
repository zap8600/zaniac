#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H 1

// Initialize the console
void tty_init();

// Write a character to the console
void tty_write_ch(char ch);

// Write a string to the console
void tty_write_str(const char* str);

#endif