#include <kernel/arch/x86_64/ps2.h>
#include <kernel/arch/x86_64/asm.h>
// #include <kernel/input.h>
#include <kernel/tty.h>

#include <stddef.h>

static const char scancode_table[] = {
    '?', // 0
    '?', // ESC
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
    '?', // Backspace
    '?', // Tab
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']',
    '?', // Enter
    '?', // Left CTRL
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`',
    '?', // Left shift
    '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/',
    '?', // Right shift
    '?', // Keypad *
    '?', // Left Alt
    ' '
};

void ps2_handler() {
    unsigned char ch = inb(0x60);
    if(ch == 0x1c) {
        // input_buf[available_chs++] = '\n';
        tty_write_ch('\n');
    } else {
        // input_buf[available_chs++] = scancode_table[ch];
        tty_write_ch(scancode_table[ch]);
    }
}