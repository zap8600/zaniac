#! /bin/sh
dd if=/dev/zero of=./uefi.img bs=512 count=93750
parted ./uefi.img -s -a minimal mklabel gpt
parted ./uefi.img -s -a minimal mkpart EFI FAT16 2048s 93716s
parted ./uefi.img -s -a minimal toggle 1 boot
dd if=/dev/zero of=./part.img bs=512 count=91669
mformat -i ./part.img -h 32 -t 32 -n 64 -c 1
mcopy -i ./part.img ./main.efi ::
dd if=./part.img of=./uefi.img bs=512 count=91669 seek=2048 conv=notrunc