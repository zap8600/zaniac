#include <kernel/arch/arch.h>
#include <kernel/tty.h>
#include <kernel/sysparam.h>
#include <kernel/arch/x86_64/asm.h>

#include <string.h>

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
    arch_init(params);
    tty_write_str("Hello, world!\n");
    unsigned long long cr3 = getcr3();
    tty_write_str(ptrtostr(cr3));
    tty_write_ch('\n');
    while(1) { asm volatile("hlt"); } // Wait for an interrupt
}