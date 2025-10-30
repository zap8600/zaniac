#include <kernel/arch/arch.h>
#include <kernel/tty.h>
#include <kernel/sysparam.h>
#include <kernel/arch/x86_64/asm.h>

void kernel_main(sysparam_t* params) {
    tty_early_init(params);
    arch_init(params);
    tty_write_str("Hello, world!\n");
    while(1) {} // Wait for an interrupt
}