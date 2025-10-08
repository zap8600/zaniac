CFILES = $(wildcard *.c)
LDFLAGS = -T linker.ld -nostdlib

CFLAGS = -ffreestanding -Wall -Wextra -mno-red-zone

boot.o:
	x86_64-elf-as boot.s -o boot.o

kernel.o:
	clang -target x86_64-elf -c $(CFILES) -o $@ $(CFLAGS)

zaniac.elf: boot.o kernel.o
	x86_64-elf-ld $(LDFLAGS) -o $@ boot.o kernel.o

clean: zaniac.elf
	rm *.o zaniac.elf