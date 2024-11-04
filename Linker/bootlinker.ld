ENTRY(entry)
OUTPUT_FORMAT("binary")
SECTIONS {
    . = 0xD2FD;
    .entry :
    {
        *(.entry)
    }
    .text :
    {
        *(.text)
    }
    .rodata :
    {
        *(.rodata)
    }
    .data :
    {
        *(.data)
    }
    .bss :
    {
        __bss_start = .;
        *(.bss)
        __end = .;
    }
}
