; Boot loader that goes into 32 bit protected mode

bits 16 ; starting in 16 bit mode
org 0x7c00 ; bios loads bootloader to this address, setting offset

bootloader:
    mov ax, 0x2401
    int 0x15 ; enable A20

    mov ax, 0x3
    int 0x10 ; set vga text mode to a known value

    ; gaining access to 32 bit registers by entering protected mode (Need a GDT to enter protected mode)

    ; setting up Global Descriptor Table

    lgdt [GDT_pointer] ; loading the GDT 
    mov eax, cr0
    or eax, 0x1 ; setting the protected mode bit 
    mov cr0, eax
    jmp CODE_SEGMENT:protected_mode

; Defining GDT: provides a structured way for the CPU to access memory sections
GDT_start:
    dq 0x0                       ; Null descriptor (must be the first entry)

; Code Segment Descriptor
GDT_Code_Section:
    dw 0xFFFF                    ; Limit (size of segment - 1) - 64KB
    dw 0x0                       ; Base Address (lower 16 bits)
    db 0x0                       ; Base Address (higher 8 bits)
    db 10011010b                 ; Access byte:
                                  ; 1 - Present
                                  ; 00 - Privilege level 0 (highest)
                                  ; 1 - Code segment
                                  ; 1 - Executable
                                  ; 1 - Direction bit (grows down)
                                  ; 0 - Readable
                                  ; 00 - Accessed (initially 0)
    db 11001111b                 ; Flags byte:
                                  ; 1 - Granularity (4KB)
                                  ; 1 - Size (32-bit segment)
                                  ; 00 - Reserved
                                  ; 1111 - Limit high (combined with low limit)
    db 0x0                       ; Reserved byte

; Data Segment Descriptor
GDT_Data_Section:
    dw 0xFFFF                    ; Limit (size of segment - 1) - 64KB
    dw 0x0                       ; Base Address (lower 16 bits)
    db 0x0                       ; Base Address (higher 8 bits)
    db 10010010b                 ; Access byte:
                                  ; 1 - Present
                                  ; 00 - Privilege level 0 (highest)
                                  ; 0 - Data segment
                                  ; 0 - Not executable
                                  ; 1 - Direction bit (grows up)
                                  ; 1 - Writable
                                  ; 00 - Accessed (initially 0)
    db 11001111b                 ; Flags byte:
                                  ; 1 - Granularity (4KB)
                                  ; 1 - Size (32-bit segment)
                                  ; 00 - Reserved
                                  ; 1111 - Limit high (combined with low limit)

GDT_end:                        ; End of GDT

GDT_pointer:
    dw GDT_end - GDT_start ; length of GDT
    dd GDT_start ; pointer to structure
CODE_SEGMENT equ GDT_Code_Section - GDT_start ; offset to code section
DATA_SEGMENT equ GDT_Data_Section - GDT_start ; offset to data section

bits 32
protected_mode:
    ;setting up segment registers
    mov ax, DATA_SEGMENT
    mov ds, ax ; points to data segment
    mov es, ax ; extra
    mov fs, ax ; extra
    mov gs, ax ; extra
    mov ss, ax ; points to stack segment

    ; printing to screen using VGA
    mov esi, hello ; load string address into esi
    mov ebx, 0xb8000 ; VGA buffer address
    mov ah, 0x0f ; set color to white on black
    
    .loop:
        lodsb ; load byte at esi address into al, increments esi after loading
        cmp al, 0 ; check if end of stirng
        jz halt
        ; al is the lower 8 bit value of eax, ax is the lower 16 bit of eax
        or eax, 0x0100 ; combine char with attribute, set the top 8 bits of ax to atribute, as lower 8 bits contain char for al
        mov word [ebx], ax ; put the char into VGA buffer
        add ebx, 2 ; increment 2 bytes, VGA first byte is char second is atribute
        jmp .loop

halt:
    cli
    hlt

hello: db "Welcome to TwistedOS",0