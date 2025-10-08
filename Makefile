subdirs:
	make -C boot main.efi
	make -C kernel zaniac.elf

uefi.img: subdirs
	dd if=/dev/zero of=./uefi.img bs=512 count=93750
	parted ./uefi.img -s -a minimal mklabel gpt
	parted ./uefi.img -s -a minimal mkpart EFI FAT16 2048s 93716s
	parted ./uefi.img -s -a minimal toggle 1 boot
	dd if=/dev/zero of=./part.img bs=512 count=91669
	mformat -i ./part.img -h 32 -t 32 -n 64 -c 1
	mcopy -i ./part.img ./boot/main.efi ::
	mcopy -i ./part.img ./kernel/zaniac.elf ::
	dd if=./part.img of=./uefi.img bs=512 count=91669 seek=2048 conv=notrunc
	rm ./part.img

clean: uefi.img
	rm uefi.img

run: uefi.img
	qemu-system-x86_64 -cpu qemu64 -drive file=uefi.img,if=ide -serial file:serial.log -drive if=pflash,format=raw,unit=0,file=/usr/share/OVMF/OVMF_CODE_4M.fd,readonly=on -drive if=pflash,format=raw,unit=1,file=/usr/share/OVMF/OVMF_VARS_4M.fd -net none

clean: uefi.img
	rm uefi.img
	make -C boot clean
	make -C kernel clean