#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef struct psf1header {
    uint16_t magic;
    uint8_t fontmode;
    uint8_t charactersize;
} psf1header;

int main(int argc, char** argv) {
    if(argc != 2) {
        fprintf(stderr, "Usage: %s [psf font]\n", argv[0]);
        return 1;
    }

    FILE* fontfile = fopen(argv[1], "rb");
    psf1header header = {0};
    fread(&header, sizeof(psf1header), 1, fontfile);
    puts("Flags: ");
    if(header.fontmode & 1) {
        puts("512 glyphs");
    } else {
        puts("256 glyphs");
    }
}