#include "sysparam.h"
#include "gdt.h"
#include "idt.h"
#include "disp.h"
#include "string.h"
#include "asm.h"

sysparam_t params;

__attribute__((naked))
void start() {
    asm volatile(
        "mov $stacktop, %rsp\n"
        "call kernelmain\n"
        ".section .bss\n"
        ".align 16\n"
        "stackbottom:\n"
        ".skip 16384\n"
        ".global stacktop\n"
        "stacktop:\n"
        ".section .text\n"
    );
}

void kernelmain(sysparam_t* bootparams) {
    asm volatile("cli");
    memcpy(&params, bootparams, sizeof(sysparam_t));
    initdisp(params.framebuffer, params.hres, params.vres, params.pitch);
    initgdt();
    initidt();
    drawcheckerboardpattern(0x0000ffff);
    char cpuidbuf[13] = {0};
    unsigned int a = 0;
    unsigned int b = 0;
    unsigned int c = 0;
    unsigned int d = 0;
    cpuid(0, &a, &b, &c, &d);
    //b = (b & 0xff) << 24 | (b & 0xff00) << 8| (b & 0xff0000) >> 8 | (b & 0xff000000) >> 24;
    //c = (c & 0xff) << 24 | (c & 0xff00) << 8| (c & 0xff0000) >> 8 | (c & 0xff000000) >> 24;
    //d = (d & 0xff) << 24 | (d & 0xff00) << 8| (d & 0xff0000) >> 8 | (d & 0xff000000) >> 24;
    memcpy(&(cpuidbuf[0]), &b, 4);
    memcpy(&(cpuidbuf[4]), &d, 4);
    memcpy(&(cpuidbuf[8]), &c, 4);
    wstrscr(&(cpuidbuf[0]));
    wchscr('\n');
    asm volatile("int $3");
    wstrscr("Still running!\n");
    while(1) asm volatile("hlt");
}
