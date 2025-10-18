#include <kernel/arch/arch.h>
#include <kernel/tty.h>
#include <kernel/sysparam.h>

void kernel_main(sysparam_t* params) {
    framebuffer_init(&(params->framebufferinfo));
    tty_init();
    arch_init();
    tty_write_str("Hello, world!\n");
    while(1) { asm volatile("hlt"); } // Wait for an interrupt
}