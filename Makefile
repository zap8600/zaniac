ARCH?=x86_64
export ARCH
SYSROOT?=$(shell pwd)/sysroot
DESTDIR:=$(SYSROOT)
export DESTDIR

.PHONY: clean

sysroot:
	mkdir -p $(SYSROOT)

efi: sysroot
	make -C boot/$(ARCH)/efi install-headers
	make -C boot/$(ARCH)/efi install

kernel: sysroot
    make -C kernel install-headers
	make -C kernel install

libc: sysroot
	make -C libc install-headers
	make -C libc install

clean:
	make -C x86_64/efi clean
	make -C kernel clean
	make -C libc clean