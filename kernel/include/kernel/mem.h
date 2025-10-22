#ifndef _KERNEL_MEM_H
#define _KERNEL_MEM_H 1

// This free marker should work across different word lengths

typedef struct memmarker_t {
    void* prev_free_addr;
    void* next_free_addr;
    unsigned long int size;
    unsigned char last;
} memmarker_t;

#endif