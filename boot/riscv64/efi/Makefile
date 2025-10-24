DESTDIR?=
INCLUDEDIR?=/lib/include
BOOTDIR?=/boot
LIBDIR?=/lib

CFLAGS?=-fshort-wchar -fno-strict-aliasing -ffreestanding -fno-stack-protector -fno-stack-check -Wno-builtin-requires-header -Wno-incompatible-library-redeclaration -Wno-long-long
CPPFLAGS?=-D__is_efi -I$(DESTDIR)$(INCLUDEDIR)
LDFLAGS?=-flavor link -subsystem:efi_application -nodefaultlib -dll -entry:inituefi
LIBS?=

.PHONY: install install-headers install-efi clean

BOOTRISCV64.efi:
	clang --target=riscv64-pc-win32-coff $(CFLAGS) $(CPPFLAGS) main.c -c -o main.o
	lld $(LDFLAGS) main.o
	mv main.dll BOOTX64.efi

clean:
	rm -f *.o *.lib *.efi

install: install-efi

install-efi: BOOTRISCV64.efi
	mkdir -p $(DESTDIR)$(BOOTDIR)/efi/boot
	cp BOOTRISCV64.efi $(DESTDIR)$(BOOTDIR)/efi/boot