LDFLAGS = -T kernel/linker.ld -nostdlib

subdirs:
    make -C boot main.efi
	make -C kernel kernel.o

zaniac.elf: kernel/kernel.o kernel/boot.o boot/main.efi
    ld.lld $(LDFLAGS) -o $@ $<