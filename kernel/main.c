#include "sysparam.h"
#include "page.h"

void* memcpy(void* d, void* s, unsigned long long n) {
    unsigned char* dst = (unsigned char*)d;
    const unsigned char* src = (const unsigned char*)s;
    for(unsigned long long i = 0; i < n; i++) {
        dst[i] = src[i];
    }
    return d;
}

sysparam_t params;

void kernel_main(sysparam_t* bootparams) {
    memcpy(&params, bootparams, sizeof(sysparam_t));
    asm volatile("cli");
    while(1) asm volatile("hlt");
}