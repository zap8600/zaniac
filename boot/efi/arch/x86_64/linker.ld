OUTPUT_FORMAT("elf64-x86-64", "elf64-x86-64", "elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)
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

    . = ALIGN(4K);

    _data = .;
    .dynamic : { *(.dynamic) }
    . = ALIGN(4K);
    .data :
    {
        *(.sdata)
        *(.data)
        *(.got)

        . = ALIGN(16);
        *(.bss)
        *(COMMON)
        . = ALIGN(16);
        _evdata = .;
        . = ALIGN(4K);
        _edata = .;
    }
    _data_vsize = _evdata - _data;
    _data_size = _edata - _data;

    . = ALIGN(4K);

    _reloc = .;
    .reloc :
    {
        *(.reloc)
        _evreloc = .;
        . = ALIGN(4K);
        _ereloc = .;
    }
    _reloc_vsize = _evreloc - _reloc;
    _reloc_size = _ereloc - _reloc;

    . = ALIGN(4K);

    _rodata = .;
    .rela : {
        *(.rela.dyn)
        *(.rela.data)
        *(.rela.text)
    }
    . = ALIGN(4K);
    .rodata : {
        *(.rodata*)
        _evrodata = .;
        . = ALIGN(4K);
        _erodata = .;
    }
    _rodata_vsize = _evrodata - _rodata;
    _rodata_size = _erodata - _rodata;
    _image_end = .;
    _alldata_size = _image_end - _reloc;

    . = ALIGN(4K);
    .dynsym : { *(.dynsym) }
    . = ALIGN(4K);
    .dynstr : { *(.dynstr) }
    . = ALIGN(4K);
    .hash : { *(.hash) }
    . = ALIGN(4K);
    .gnu.hash : { *(.gnu.hash) }

    .comment 0 : { *(.comment) }
}