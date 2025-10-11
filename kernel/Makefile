LDFLAGS = -T linker.ld -nostdlib -z noexecstack

CFLAGS = -ffreestanding -Wall -Wextra -mno-red-zone -g

OBJS=\
main.o\
disp.o\
string.o\
asm.o\
gdt.o\
idt.o\
pic.o\

.PHONY: clean
.SUFFIXES: .o .c .s

zaniac.elf: $(OBJS)
	ld.lld $(LDFLAGS) -o $@ $(OBJS)
	llvm-objcopy --only-keep-debug zaniac.elf zaniac.sym
	llvm-objcopy --strip-debug zaniac.elf

clean:
	rm -f *.o zaniac.elf zaniac.sym

.c.o:
	clang -target x86_64-elf -c $< -o $@ $(CFLAGS)
