DESTDIR?=
INCLUDEDIR?=/lib/include
BOOTDIR?=/boot
LIBDIR?=/lib

CFLAGS?=-fshort-wchar -fno-strict-aliasing -ffreestanding -fno-stack-protector -fno-stack-check -mno-red-zone -Wno-builtin-requires-header -Wno-incompatible-library-redeclaration -Wno-long-long
CPPFLAGS?=-D__is_efi -I$(DESTDIR)$(INCLUDEDIR)
LDFLAGS?=-flavor link -subsystem:efi_application -nodefaultlib -dll -entry:inituefi -L $(DESTDIR)$(LIBDIR) -lefi
LIBS?=

.PHONY: install install-headers install-efi clean

BOOTX64.efi:
	clang -target=x86_64-pc-win32-coff $(CFLAGS) $(CPPFLAGS) main.c -c -o main.o
	lld $(LDFLAGS) main.o
	mv main.dll BOOTX64.efi

clean:
	rm -f *.o *.lib *.efi

install: install-headers install-efi

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-efi: BOOTX64.efi
	mkdir -p $(DESTDIR)$(BOOTDIR)/efi/boot
	cp BOOTX64.efi $(DESTDIR)$(BOOTDIR)/efi/boot