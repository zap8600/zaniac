#include "gdt.h"

typedef struct gdtentry_t {
    unsigned short int limitlow;
    unsigned short int baselow;
    unsigned char basemiddle;
    unsigned char access;
    unsigned char granularity;
    unsigned char basehigh;
} __attribute__((packed)) gdtentry_t;