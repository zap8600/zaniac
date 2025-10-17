__attribute__((naked))
void start() {
    asm volatile(
        "movabs $stacktop, %rsp\n"
        "call archinit\n"
        "cli\n"
        "1:\n"
        "hlt\n"
        "jmp 1b\n"
        ".section .bss\n"
        ".align 16\n"
        "stackbottom:\n"
        ".skip 16384\n"
        ".global stacktop\n"
        "stacktop:\n"
        ".section .text\n"
    );
}

void archinit() {
    // TODO: Set up stuff for architecture
}