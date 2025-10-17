ARCH?=x86_64
HOST:=$(ARCH)-elf
# TODO: Have some way of doing more than just ELF targets

CFLAGS?=-g
CPPFLAGS?=
LDFLAGS?=
LIBS?=

# TODO: Directory stuff

CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -D__is_kernel -Iinclude
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib

ARCHDIR=arch/$(ARCH)

include $(ARCHDIR)/make.config

CFLAGS:=$(CFLAGS) $(KERNEL_ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(KERNEL_ARCHCPP_FLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS:=$(LIBS) $(KERNEL_ARCH_LIBS)

KERNEL_OBJS=\
$(KERNEL_ARCH_OBJS) \
kernel/kernel.o \
kernel/framebuffer.o \
kernel/tty.o \
kernel/string.o \

OBJS=\
$(KERNEL_OBJS) \

LINK_LIST=\
$(LDFLAGS) \
$(KERNEL_OBJS) \
$(LIBS) \

.PHONY: all clean
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
	rm -f $(OBJS) *.o */*.o */*/*.o
