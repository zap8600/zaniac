LDFLAGS = -T linker.ld -nostdlib -z noexecstack

CFLAGS=-ffreestanding -Wall -Wextra -mno-red-zone -g

ARCH?=x86_64

include $(ARCH)/make.config

CFLAGS:=$(CFLAGS) $(ARCHCFLAGS)

OBJS=\
main.o\
disp.o\
string.o\
$(ARCH)/arch.o\

.PHONY: clean
.SUFFIXES: .o .c

zaniac.elf: $(OBJS)
	x86_64-elf-ld $(LDFLAGS) -o $@ $(OBJS)
	x86_64-elf-objcopy --only-keep-debug zaniac.elf zaniac.sym
	x86_64-elf-objcopy --strip-debug zaniac.elf

clean:
	rm -f *.o zaniac.elf zaniac.sym
	make -C $(ARCH) clean

.c.o:
	clang -target x86_64-elf -c $< -o $@ $(CFLAGS)

.s.o:
	x86_64-elf-as $< -o $@