#ifndef _KERNEL_MEM_H
#define _KERNEL_MEM_H 1

typedef struct freemarker_t {
    unsigned long int prev_free_addr;
    unsigned long int next_free_addr;
    unsigned long int size;
} freemarker_t;

#endif