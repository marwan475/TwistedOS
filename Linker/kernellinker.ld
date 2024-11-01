ENTRY(kernel_main)
OUTPUT_FORMAT("binary")
SECTIONS {
    . = 0x00100000;
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
