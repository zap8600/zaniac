.section text
.align 8

.macro IRQ index byte
    .global _irq\index
    .type _irq\index, @function
    _irq\index:
        pushq $0x00
        pushq $\byte
        jmp isrcommon
.endm

.macro ISRNOERR index
    .global _isr\index
    .type _isr\index, @function
    _isr\index:
        pushq $0x00
        pushq $\index
        jmp isrcommon
.endm

.macro ISRERR index
    .global _isr\index
    .type _isr\index, @function
    _isr\index:
        pushq $\index
        jmp isrcommon
.endm

ISRNOERR 0
ISRNOERR 1
ISRNOERR 2
ISRNOERR 3
ISRNOERR 4
ISRNOERR 5
ISRNOERR 6
ISRNOERR 7
ISRERR 8
ISRNOERR 9
ISRERR 10
ISRERR 11
ISRERR 12
ISRERR 13
ISRERR 14
ISRNOERR 15
ISRNOERR 16
ISRERR 17
ISRNOERR 18
ISRNOERR 19
ISRNOERR 20
ISRNOERR 21
ISRNOERR 22
ISRNOERR 23
ISRNOERR 24
ISRNOERR 25
ISRNOERR 26
ISRNOERR 27
ISRNOERR 28
ISRNOERR 29
ISRERR 30
ISRNOERR 31

//
IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47
//

isrcommon:
    /* Saving all the registers */
    push %rax
    push %rbx
    push %rcx
    push %rdx
    push %rsi
    push %rdi
    push %rbp
    push %r8
    push %r9
    push %r10
    push %r11
    push %r12
    push %r13
    push %r14
    push %r15

    /* Apparently this is required by the SysV abi */
    /* DF must be clear on function entry */
    cld

    call inthandler

    /* Now we return them */
    push %r15
    push %r14
    push %r13
    push %r12
    push %r11
    push %r10
    push %r9
    push %r8
    push %rbp
    push %rdi
    push %rsi
    push %rdx
    push %rcx
    push %rbx
    push %rax

    iretq
