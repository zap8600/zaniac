void istuff() __attribute__((naked)) __attribute__((used));
void istuff() {
    asm volatile(
        ".section .init\n"
        ".global _init\n"
        "_init:\n"
        "push %rbp\n"
        "movq %rsp, %rbp\n"
        ".section .fini\n"
        ".global _fini\n"
        "_fini:\n"
        "push %rbp\n"
        "movq %rsp, %rbp\n"
        ".section .text\n"
    );
}