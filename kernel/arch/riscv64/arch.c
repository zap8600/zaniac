#include <kernel/tty.h>
#include <kernel/arch/arch.h>

__attribute__((naked))
void bootstrap() {
    asm volatile(
        "    .globl _start\n"
        "_start:\n"
        "    la sp, stack_top\n"
        "    call kernel_main\n"
        // TODO: Disable interrputs
        "    1:\n"
        "    wfi\n"
        "    j 1b\n"
        "    .section .bss\n"
        "    .align 16\n"
        "stack_bottom:\n"
        "    .skip 16384\n"
        "    .globl stack_top\n"
        "stack_top:\n"
        "    .section .text\n"
    );
}

void arch_init(sysparam_t* params) {
    return; // Nothing to do here yet.
}
