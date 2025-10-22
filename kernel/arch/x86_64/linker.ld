OUTPUT_FORMAT(elf64-x86-64)
ENTRY(start)
SECTIONS {
    . = 0xC0000000;
    .text : ALIGN(4K)
    {
        *(.text)
    }
    .rodata : ALIGN(4K)
    {
        *(.rodata)
    }
    .data : ALIGN(4K)
    {
        virt_data_start = .;
        *(.data)
    }
    .bss : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }
}
