// RISC-V64 test

#include <boot/efi.h>

void wchscr(const char c) {
    *((unsigned char*)0x10000000) = (unsigned char)c;
}

void wstr(const char* s) {
    char c = 0;
    while((c = *s++)) {
        wchscr(c);
    }
}

void* memcpy(void* d, void* s, unsigned long long n) {
    unsigned char* dst = (unsigned char*)d;
    const unsigned char* src = (const unsigned char*)s;
    for(unsigned long long i = 0; i < n; i++) {
        dst[i] = src[i];
    }
    return d;
}

unsigned long long inituefi(void* image, efisystemtable_t* systab) {
    // asm volatile(
    //     "_loop_print:\n"
    //     "    addi a0, zero, 72\n" // set a0 to 'H'
    //     "    addi a1, zero, 1\n" // set a1 to 1
    //     "    call wchscr\n" // Print character
    //     "    bne a1, a0, _loop_print\n" 
    // );

    // unsigned long long status = 0;

    wstr("Hello, RISC-V world!\n");
    
    // while(1) { wchscr(72); }

    return 0;
}