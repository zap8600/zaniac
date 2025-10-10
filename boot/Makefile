main.efi:
	clang -fshort-wchar -fno-strict-aliasing -ffreestanding -fno-stack-protector -fno-stack-check -mno-red-zone --target=x86_64-pc-win32-coff -Wno-builtin-requires-header -Wno-incompatible-library-redeclaration -Wno-long-long main.c -c -o main.o
	lld -flavor link -subsystem:efi_application -nodefaultlib -dll -entry:inituefi main.o
	rm main.lib
	mv main.dll main.efi
	rm main.o

.PHONY: clean

clean: main.efi
	rm main.efi
