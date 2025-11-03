ARCH?=x86_64
HOST:=$(ARCH)-elf

DESTDIR?=
INCLUDEDIR?=/lib/include
BOOTDIR?=/boot
LIBDIR?=/lib

CFLAGS?=-fpic -fPIC -fshort-wchar -fno-strict-aliasing -ffreestanding -fno-stack-protector -fno-stack-check -Werror-implicit-function-declaration -Wshadow -Wunused -Wall -Werror
CPPFLAGS?=-D__is_efi -I$(DESTDIR)$(INCLUDEDIR)
LDFLAGS?=-s -Bsymbolic -shared # --orphan-handling=error
LIBS?=-nostdlib

ARCHDIR=arch/$(ARCH)

include $(ARCHDIR)/make.config

CFLAGS:=$(CFLAGS) $(ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(ARCH_CPPFLAGS)

OBJS=\
main.o \
$(ARCH_OBJS) \

LINK_LIST=\
$(LDFLAGS) \
$(OBJS) \
$(LIBS) \

.PHONY: install install-efi clean disk bootloader run run-x86_64 run-riscv64
.SUFFIXES: .c .o .elf

.c.o:
	clang -target $(HOST) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

boot.elf: $(OBJS) $(ARCHDIR)/linker.ld
	ld.lld -T $(ARCHDIR)/linker.ld -o $@ $(LINK_LIST)

bootloader: boot.elf
	llvm-objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .dynstr -j .rel* -j .rela* -j .reloc -O binary boot.elf $(ARCH_BOOT_PREFIX).efi

clean:
	rm -f *.o *.elf *.efi *.img $(OBJS)

install: install-efi

install-efi: bootloader
	mkdir -p $(DESTDIR)$(BOOTDIR)/efi/boot
	cp $(ARCH_BOOT_PREFIX).efi $(DESTDIR)$(BOOTDIR)/efi/boot

disk:
	dd if=/dev/zero of=./uefi.img bs=512 count=93750
	parted ./uefi.img -s -a minimal mklabel gpt
	parted ./uefi.img -s -a minimal mkpart EFI FAT16 2048s 93716s
	parted ./uefi.img -s -a minimal toggle 1 boot
	dd if=/dev/zero of=./part.img bs=512 count=91669
	mformat -i ./part.img -h 32 -t 32 -n 64 -c 1
	mcopy -i ./part.img $(DESTDIR)/boot/efi ::
	mcopy -i ./part.img $(DESTDIR)/boot/efi/boot ::efi
	mcopy -i ./part.img $(DESTDIR)/boot/efi/boot/* ::efi/boot
#	mcopy -i ./part.img $(DESTDIR)/boot/zaniac.elf ::
	dd if=./part.img of=./uefi.img bs=512 count=91669 seek=2048 conv=notrunc

run-x86_64:
	qemu-system-x86_64 -cpu qemu64 -drive file=uefi.img,if=ide -serial file:serial.log -drive if=pflash,format=raw,unit=0,file=../../firmware/OVMF_CODE.fd,readonly=on -drive if=pflash,format=raw,unit=1,file=../../firmware/OVMF_VARS.fd -net none -d cpu_reset -D debug.log -vnc :1

run-riscv64:
	qemu-system-riscv64 -M virt,acpi=off -drive if=pflash,format=raw,unit=0,file=/usr/share/qemu-efi-riscv64/RISCV_VIRT_CODE.fd,readonly=on -drive if=pflash,format=raw,unit=1,file=../../firmware/RISCV_VIRT_VARS.fd -net none -nographic -drive file=./uefi.img,format=raw,if=virtio

run: run-$(ARCH)