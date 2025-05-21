    .align 4
    .globl _start
    .text
_start:
    // Load the ELF data into arguments
    // rip is used for position-independent code

// Fake relocation info so EFI won't complain
    .data
dummy: .long 0
    .section .reloc, "a"
label1:
    .long dummy-label1
    .long 10
    .word 0

    .text

