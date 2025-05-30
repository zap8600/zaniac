void bootstrap(void) {
    __asm__ __volatile__ (
        "    .globl __chkstk"
        "__chkstk:"
        "    ret"
    );
}

typedef struct guid_t {
    unsigned int d1;
    unsigned short d2;
    unsigned short d3;
    unsigned char d4[8]; 
} guid_t;

// BS at systab + 24 + 2 + 4 + 8 + 24 + 8 + 80 + 8 + 80 + 8, 246
void* bs;
// LP at bs + 24 + 16 + 40 + 48 + 72 + 40 + 24 + 16 + 24 + 16, 320
typedef unsigned long (__attribute__((ms_abi))* lopr_t)(guid_t* pr, void* reg, void** inf);
lopr_t lp;

void inituefi(void* image, void* systab) {
    __asm__ __volatile__ (
        "    movq %cr0, %rax\n"
        "    andb $0xF1, %al\n"
        "    movq %rax, %cr0\n"
        "    movq %cr4, %rax\n"
        "    orw $3 << 9, %ax\n"
        "    mov %rax, %cr4\n"
    );
    lp = (lopr_t)((*(char** )(((char*)systab) + 246)) + 320);
}