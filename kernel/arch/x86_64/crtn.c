void nstuff() __attribute__((naked)) __attribute__((used));
void nstuff() {
    asm volatile(
        ".section .init\n"
        "popq %rbp\n"
        ".section .fini\n"
        "popq %rbp"
        ".section .text\n"
    );
}