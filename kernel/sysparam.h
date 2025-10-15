#ifndef SYSPARAM_H
#define SYSPARAM_H

typedef struct sysparam_t {
    unsigned int hres;
    unsigned int vres;
    unsigned int pitch;
    unsigned long long size;
} sysparam_t;

#endif