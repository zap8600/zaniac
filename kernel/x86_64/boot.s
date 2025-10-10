.section .bss
.align 16
stackbottom:
.skip 16384
.global stacktop
stacktop:

.section .text
.global _start
.type _start,@function
_start:
    mov $stacktop, %rsp;
    call kernelmain
    cli
1:  hlt
    jmp 1b

.size _start, . - _start
