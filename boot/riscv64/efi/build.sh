#! /bin/sh
clang -target riscv64-elf -fpic -fshort-wchar -ffreestanding -fno-stack-protector -Wshadow -Wall -Wunused -Werror-implicit-function-declaration -c main.c -o main.o
ld.lld -T linker.ld -s -Bsymbolic -nostdlib -shared main.o -o main.elf
llvm-objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .dynstr -j .rel* -j .rela* -j .reloc -O binary main.elf main.efi
# --orphan-handling=error