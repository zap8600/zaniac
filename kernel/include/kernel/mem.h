#ifndef _KERNEL_MEM_H
#define _KERNEL_MEM_H 1

// This memory marker should work across different word lengths

typedef struct memmarker_t {
    void* prev_free_addr;
    void* next_free_addr;
#ifndef _WIN32
    unsigned long int size;
#else
    unsigned long long size;
#endif
    unsigned char last;
} __attribute__((packed)) memmarker_t;

#endif