#ifndef _KERNEL_ARCH_X86_64_PIC_H
#define _KERNEL_ARCH_X86_64_PIC_H 1

void pic_remap(int offset1, int offset2);
void pic_set_mask(unsigned char irqline);
void pic_clear_mask(unsigned char irqline);
void pic_acknowledge(unsigned char irqline);

#endif