    .align 4
    .globl _start
    .text
_start:
    // Load the ELF data into arguments
    // rip is used for position-independent code
    lea ImageBase(%rip), %rdi
    lea _DYNAMIC(%rip), %rsi
    call initefi
    ret

// Fake relocation info so EFI won't complain
    .data
dummy: .long 0
    .section .reloc, "a"
label1:
    .long dummy-label1
    .long 10
    .word 0

    .text
    .globl initefi
initefi:
    pushq %rbp
    movq %rsp, %rbp
    subq $32, %rsp
    movq $0, %rax // Using this to store loop counter
_loop:
    movq (%rsi, %r8, 16), %r8
    movq $0, %r9
    cmp, %r8, %r9
    // Jump out of the loop if NULL
    // jz something, something
    

