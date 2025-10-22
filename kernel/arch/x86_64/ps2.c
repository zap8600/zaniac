#include <kernel/arch/x86_64/ps2.h>
#include <kernel/arch/x86_64/asm.h>
#include <kernel/tty.h>

#include <string.h>

const unsigned char scan_codes[] = {
    '?', '?', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '?',
    '?', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '?',
    '?', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', '?', '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', '?', '?', '?', ' '
};

void ps2_handler() {
    unsigned char scan_code = inb(0x60);
    if(((unsigned long long)scan_code) >= sizeof(scan_codes)) return;
    switch(scan_code) {
        case 0x1c: { // Enter
            tty_write_ch('\n');
            break;
        }
        default: {
            tty_write_ch(scan_codes[scan_code]);
            break;
        }
    }
}