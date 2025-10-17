#include <kernel/arch/arch.h>
#include <kernel/tty.h>
#include <kernel/sysparam.h>

void kernel_main(sysparam_t* params) {
    framebuffer_init(&(params->framebufferinfo));
    arch_init();
    tty_init();
    tty_write_str("Hello, world!\n");
}