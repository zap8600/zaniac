ARCH?=x86_64
HOST:=$(ARCH)-elf # TODO: Have some way of doing more than just ELF targets

CFLAGS?=-g
CPPFLAGS?=
LDFLAGS?=
LIBS?=

# TODO: Directory stuff

CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -D__is_kernel -Iinclude
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -lk

ARCHDIR=arch/$(HOST)

include $(ARCHDIR)/make.config

CFLAGS:=$(CFLAGS) $(KERNELARCHCFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(KERNELARCHCPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNELARCHLDFLAGS)
LIBS:=$(LIBS) $(KERNELARCHLIBS)

KERNEL_OBJS=\
$(KERNEL_ARCH_OBJS) \
kernel/kernel.o \

OBJS=\
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \

LINK_LIST=\
$(LDFLAGS) \
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(LIBS) \
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \

.PHONY: all clean
.SUFFIXES: .o .c .s

all: zaniac.elf

zaniac.elf: $(OBJS) $(ARCHDIR)/linker.ld
	ld.lld -T $(ARCHDIR)/linker.ld -o $@ $(LINK_LIST)

$(ARCHDIR)/crtbegin.o $(ARCHDIR)/crtend.o:
	OBJ=`clang -target $(HOST) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

.c.o:
	clang -target $(HOST) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

clean:
	rm -f zaniac.elf
	rm -f $(OBJS) *.o */*.o */*/*.o
