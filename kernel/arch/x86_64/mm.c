#include <kernel/arch/x86_64/mm.h>
#include <kernel/arch/x86_64/asm.h>

#include <string.h>

#define GET_FROM_TABLE4K(a) (a & 0xfffffffffffff000)
#define GET_FROM_TABLE2M(a) (a & 0xfffffffffff00000)

// Some page tables to get us started
unsigned long long* pml4; // root
unsigned long long* pdpt; // root
unsigned long long* pd; // for kernel
unsigned long long* pd1; // for framebuffer
unsigned long long* pt; // also for kernel, assuming we don't exceed 2MB

extern unsigned long long virt_data_start;
unsigned long long phys_data_start = 0;

void x86_64_mm_init(sysparam_t* params) {
    pml4 = (void*)getcr3();
    pdpt = (void*)GET_FROM_TABLE4K(pml4[0]);
}