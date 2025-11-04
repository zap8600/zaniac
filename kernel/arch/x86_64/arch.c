#include <kernel/arch/x86_64/gdt.h>
#include <kernel/arch/x86_64/idt.h>
#include <kernel/tty.h>
#include <kernel/arch/arch.h>

__attribute__((naked))
void bootstrap() {
    asm volatile(
        "    .globl _start\n"
        "_start:\n"
        "    movabs $stack_top, %rsp\n"
        "    call kernel_main\n"
        "    cli\n"
        "    1:\n"
        "    hlt\n"
        "    jmp 1b\n"
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
    gdt_init();
    tty_write_str("Initialized the Global Descriptor Table.\n");
    idt_init();
    tty_write_str("Initialized the Interrupt Descriptor Table and enabled interrupts.\n");
}