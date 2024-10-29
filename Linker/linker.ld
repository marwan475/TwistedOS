ENTRY(entry)
OUTPUT_FORMAT("binary")
SECTIONS {
    . = 0x0500;
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