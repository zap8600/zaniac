#include <kernel/arch/arch.h>
#include <kernel/tty.h>
#include <kernel/sysparam.h>

#include <string.h>

unsigned long int abs(signed long int num) {
    if(num < 0) {
        return (unsigned long int)(-num);
    } else {
        return (unsigned long int)num;
    }
}

char* numtostr(unsigned long int val, unsigned long int base) {
    unsigned char neg = 0;
    if(((signed long int)val) < 0) {
        neg = 1;
        val = abs(val);
    }
    static char buf[32] = {0};
    if(!val) {
        buf[30] = '0';
        return &buf[30];
    }
    unsigned long int i = 30;
    for(; val && i; i--, val /= base) {
        buf[i] = "0123456789abcdef"[val % base];
    }
    if(neg) {
        buf[i] = '-';
        return &buf[i];
    } else {
        return &buf[i + 1];
    }
}

char* ptrtostr(unsigned long long ptr) {
    static char ptrstrbuf[19] = {0};
    memcpy(ptrstrbuf, "0x", 2);
    for(unsigned long long i = 0, j = 15; i < 16; i++, j--) {
        ptrstrbuf[2 + j] = "0123456789abcdef"[(ptr >> (i * 4)) & 0x0f];
    }
    return &(ptrstrbuf[0]);
}

void kernel_main(sysparam_t* params) {
    tty_early_init(params);
    arch_init();
    tty_write_str("Hello, world!\n");
    memmarker_t* current_marker = params->memory_map;
    for(unsigned long int i = 0; ; i++) {
        tty_write_str("Address: ");
        tty_write_str(ptrtostr(((unsigned long long)current_marker) + sizeof(memmarker_t)));
        tty_write_str("\nSize: ");
        tty_write_str(ptrtostr(current_marker->size));
        tty_write_str("\n-----\n");
        if(current_marker->last) break;
        current_marker = current_marker->next_free_addr;
    }
    while(1) { asm volatile("hlt"); } // Wait for an interrupt
}