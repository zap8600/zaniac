uefi.img: main.efi
	dd if=/dev/zero of=./uefi.img bs=512 count=93750
	parted ./uefi.img -s -a minimal mklabel gpt
	parted ./uefi.img -s -a minimal mkpart EFI FAT16 2048s 93716s
	parted ./uefi.img -s -a minimal toggle 1 boot
	dd if=/dev/zero of=./part.img bs=512 count=91669
	mformat -i ./part.img -h 32 -t 32 -n 64 -c 1
	mcopy -i ./part.img ./main.efi ::
	dd if=./part.img of=./uefi.img bs=512 count=91669 seek=2048 conv=notrunc
	rm part.img

main.efi:
	clang -fshort-wchar -fno-strict-aliasing -ffreestanding -fno-stack-protector -fno-stack-check -mno-red-zone --target=x86_64-pc-win32-coff -Wno-builtin-requires-header -Wno-incompatible-library-redeclaration -Wno-long-long main.c -c -o main.o
	lld -flavor link -subsystem:efi_application -nodefaultlib -dll -entry:inituefi main.o
	rm main.lib
	mv main.dll main.efi
	rm main.o

clean: uefi.img
	rm uefi.img main.efi

run: uefi.img bios64.bin
	qemu-system-x86_64 -cpu qemu64 -bios ./bios64.bin -drive file=uefi.img,if=ide -serial file:serial.log

bios64.bin:
	wget https://github.com/BlankOn/ovmf-blobs/raw/refs/heads/master/bios64.bin
