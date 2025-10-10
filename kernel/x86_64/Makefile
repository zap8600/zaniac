CFLAGS=-ffreestanding -Wall -Wextra -mno-red-zone -g

include make.config

CFLAGS:=$(CFLAGS) $(ARCHCFLAGS)

OBJS=\
boot.o\
gdt.o\
asm.o\
idt.o\
isr.o\

arch.a: $(OBJS)
	$(ARCH)-elf-ar rcs $@ $(OBJS)

.PHONY: clean
.SUFFIXES: .o .c .s

clean:
	rm -f *.o arch.a

	.c.o:
	clang -target x86_64-elf -c $< -o $@ $(CFLAGS)

.s.o:
	x86_64-elf-as $< -o $@