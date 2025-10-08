CFILES = $(wildcard *.c)
SFILES = $(wildcard *.s)

CFLAGS = -ffreestanding -Wall -Wextra -mno-red-zone

boot.o:
    x86_64-elf-as boot.s -o boot.o

kernel.o:
    clang -target x86_64-elf -c $(CFILES) -o $@ $(CFLAGS)