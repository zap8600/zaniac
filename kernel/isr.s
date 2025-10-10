.section text
.align 8

.macro ISRNOERR index
    .global _isr\index
    .type _isr\index, @function
    _isr\index:
        jmp isrcommon
.endm

.macro ISRERR index
    .global _isr\index
    .type _isr\index, @function
    _isr\index:
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

isrcommon:
    call exceptionhandler
    iretq
