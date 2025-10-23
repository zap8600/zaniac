ARCH?=x86_64
export ARCH
SYSROOT?=$(shell pwd)/sysroot
DESTDIR:=$(SYSROOT)
export DESTDIR

sysroot:
	mkdir -p $(SYSROOT)

ufi: sysroot
	make -C boot/$(ARCH)/efi install-headers
	make -C boot/$(ARCH)/efi install

kernel: sysroot
    make -C kernel install-headers
	make -C kernel install