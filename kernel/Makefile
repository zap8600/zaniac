ARCH?=x86_64
HOST:=$(ARCH)-elf

CFLAGS?=-g
CPPFLAGS?=
LDFLAGS?=
LIBS?=

DESTDIR?=
INCLUDEDIR?=/lib/include
BOOTDIR?=/boot
LIBDIR?=/lib

CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -D__is_kernel -I$(DESTDIR)$(INCLUDEDIR)
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -L $(DESTDIR)$(LIBDIR) -lk

ARCHDIR=arch/$(ARCH)

include $(ARCHDIR)/make.config

CFLAGS:=$(CFLAGS) $(KERNEL_ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS:=$(LIBS) $(KERNEL_ARCH_LIBS)

KERNEL_OBJS=\
$(KERNEL_ARCH_OBJS) \
kernel/kernel.o \
kernel/framebuffer.o \
kernel/tty.o \

OBJS=\
$(KERNEL_OBJS) \

LINK_LIST=\
$(LDFLAGS) \
$(KERNEL_OBJS) \
$(LIBS) \

.PHONY: all clean install install-headers install-kernel
.SUFFIXES: .o .c .s

all: zaniac.elf

zaniac.elf: $(OBJS) $(ARCHDIR)/linker.ld
	ld.lld -T $(ARCHDIR)/linker.ld -o $@ $(LINK_LIST)
	llvm-objcopy --only-keep-debug zaniac.elf zaniac.sym
	llvm-objcopy --strip-debug zaniac.elf

# $(ARCHDIR)/crtbegin.o $(ARCHDIR)/crtend.o:
#	OBJ=`clang -target $(HOST) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

.c.o:
	clang -target $(HOST) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

clean:
	rm -f zaniac.elf
	rm -f zaniac.sym
	rm -f $(OBJS) *.o */*.o */*/*.o

install: install-headers install-kernel

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-kernel: zaniac.elf
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp zaniac.elf $(DESTDIR)$(BOOTDIR)
