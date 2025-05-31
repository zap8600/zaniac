clang -fshort-wchar -fno-strict-aliasing -ffreestanding -fno-stack-protector -fno-stack-check -mno-red-zone --target=x86_64-pc-win32-coff -Wno-builtin-requires-header -Wno-incompatible-library-redeclaration -Wno-long-long boot.c -c -o boot.o
lld -flavor link -subsystem:efi_application -nodefaultlib -dll -entry:inituefi boot.o
rm boot.lib
sudo losetup --offset 1048576 --sizelimit 46934528 /dev/loop32 ./uefi.img
sudo mount /dev/loop32 /mnt
sudo cp boot.dll /mnt/main.efi
rm boot.dll
sudo ls /mnt
sudo umount /mnt
sudo losetup -d /dev/loop32