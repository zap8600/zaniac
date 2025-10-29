ARCH?=x86_64

DESTDIR?=
INCLUDEDIR?=/lib/include
BOOTDIR?=/boot
LIBDIR?=/lib

CFLAGS?=-fpic -fPIC -fshort-wchar -fno-strict-aliasing -ffreestanding -fno-stack-protector -fno-stack-check -mno-red-zone -Wno-builtin-requires-header -Wno-incompatible-library-redeclaration -Wno-long-long
CPPFLAGS?=-D__is_efi -I$(DESTDIR)$(INCLUDEDIR)
LDFLAGS?=-T $(ARCH)/linker.ld -s -Bsymbolic -nostdlib -shared
LIBS?=

.PHONY: install install-efi clean

BOOT.efi:
	clang -target=$(ARCH)-elf $(CFLAGS) $(CPPFLAGS) main.c $(ARCH)/bootstrap.o -c -o main.o
	ld.lld $(LDFLAGS) main.o -o main.elf
	llvm-objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .dynstr -j .rel* -j .rela* -j .reloc -O binary main.elf main.efi
	mv main.efi BOOT.efi

clean:
	rm -f *.o *.lib *.efi

install: install-efi

install-efi: BOOT.efi
	mkdir -p $(DESTDIR)$(BOOTDIR)/efi/boot
	cp BOOT.efi $(DESTDIR)$(BOOTDIR)/efi/boot