LDFLAGS = -T linker.ld -nostdlib -z noexecstack

CFLAGS = -ffreestanding -Wall -Wextra -mno-red-zone -g

OBJS=\
main.o\
boot.o\
gdt.o\
idt.o\
isr.o\
disp.o\
string.o\
asm.o\

.PHONY: clean
.SUFFIXES: .o .c .s

zaniac.elf: $(OBJS)
	x86_64-elf-ld $(LDFLAGS) -o $@ $(OBJS)
	x86_64-elf-objcopy --only-keep-debug zaniac.elf zaniac.sym
	x86_64-elf-objcopy --strip-debug zaniac.elf

clean:
	rm -f *.o zaniac.elf zaniac.sym

.c.o:
	clang -target x86_64-elf -c $< -o $@ $(CFLAGS)

.s.o:
	x86_64-elf-as $< -o $@