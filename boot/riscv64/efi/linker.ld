OUTPUT_FORMAT("elf64-littleriscv", "elf64-littleriscv", "elf64-littleriscv")
OUTPUT_ARCH(riscv)
ENTRY(_start)
SECTIONS {
    .text 0 :
    {
        *(.text.head)
        . = ALIGN(4K);
        _text = .;
        *(.text)
        . = ALIGN(16);
        _evtext = .;
        . = ALIGN(4K);
        _etext = .;
    }
    _text_vsize = _evtext - _text;
    _text_size = _etext - _text;
    .data :
    {
        *(.data)
        *(.sdata)
        *(COMMON)
        . = ALIGN(16);
        *(.bss)
        . = ALIGN(16);
    }
}