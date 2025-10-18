#include <kernel/input.h>

#include <stddef.h>
#include <string.h>

static char buf[512] = {0};

char* input_buf = buf;
size_t available_chs = 0;

void input_init() {
    // TODO: Something
}

char input_get_ch() {
    while(!available_chs) {}
    char ch = input_buf[0];
    memcpy(input_buf, input_buf + 1, (--available_chs));
    input_buf[available_chs] = '\0';
    return ch;
}