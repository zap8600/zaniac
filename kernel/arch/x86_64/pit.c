#include <kernel/arch/x86_64/pit.h>
#include <kernel/arch/x86_64/asm.h>

#include <stdint.h>

#define PITCMD 0x43
#define CHANNEL0 0x40

void pit_init() {
    uint8_t value = 0;

    value &= 0x3f; // Set Channel 0
    value |= 0x3 << 4; // Low & High
}

void pit_handler() {
    //
}