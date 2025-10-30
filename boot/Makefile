ARCH?=x86_64
PLATFORM?=efi

DESTDIR?=
INCLUDEDIR?=/lib/include
BOOTDIR?=/boot
LIBDIR?=/lib

.PHONY: install install-bootloader install-headers clean disk

install: install-headers install-bootloader

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-bootloader:
	make -C $(PLATFORM) install

clean:
	make -C $(PLATFORM) clean

disk:
	make -C $(PLATFORM) disk