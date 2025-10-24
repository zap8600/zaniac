ARCH?=x86_64
export ARCH
SYSROOT?=$(shell pwd)/sysroot
DESTDIR:=$(SYSROOT)
export DESTDIR

.PHONY: clean kernel libc bootloader kernel-headers boot-headers libc-headers

sysroot:
	mkdir -p $(SYSROOT)

kernel-headers: sysroot
	make -C kernel install-headers

boot-headers: sysroot
	make -C boot install-headers

libc-headers: sysroot
	make -C libc install-headers

bootloader: boot-headers kernel-headers libc-headers
	make -C boot install

kernel: kernel-headers libc-headers libc
	make -C kernel install

libc: kernel-headers libc-headers
	make -C libc install

clean:
	make -C boot clean
	make -C kernel clean
	make -C libc clean
	rm -rf sysroot