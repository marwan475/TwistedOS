; Boot loader that goes into 32 bit protected mode

section .bootloader ; telling the linker what to put at the start of the binary
bits 16 ; starting in 16 bit mode
global bootloader

global GDT_CODE_SEGMENT
global GDT_DATA_SEGMENT

bootloader: ; Real mode
    mov ax, 0x2401
    int 0x15 ; enable A20

    mov ax, 0x3
    int 0x10 ; set vga text mode to a known value

    ; reading from disk, past the boot sector using BIOS interupt
    mov [DISK],dl ; store drive number which is in dl

	mov ah, 0x2    ; BIOS function read sectors
	mov al, 250     ; how many sectors to read
	mov ch, 0      ; what cylinder to read from 
	mov dh, 0      ; what head to read from
	mov cl, 2      ; what sector to read from
	mov dl, [DISK] ; drive number
	mov bx, disk_sector ; where to load the sector
	int 0x13 ; calling interupt

	cli

    ; gaining access to 32 bit registers by entering protected mode (Need a GDT to enter protected mode)

    ; setting up Global Descriptor Table

    lgdt [GDT_pointer] ; loading the GDT 
    mov eax, cr0
    or eax, 0x1 ; setting the protected mode bit 
    mov cr0, eax

    ; Protected mode starts, cant use BIOS functions in protected mode

    ;setting up segment registers
    mov ax, GDT_DATA_SEGMENT
    mov ds, ax ; points to data segment
    mov es, ax ; extra
    mov fs, ax ; extra
    mov gs, ax ; extra
    mov ss, ax ; points to stack segment

    jmp GDT_CODE_SEGMENT:protected_mode

; Defining GDT: provides a structured way for the CPU to access memory sections , setting the rules of memory access
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
    db 0x0                       ; Reserved byte       
GDT_end:                        ; End of GDT

GDT_pointer:
    dw GDT_end - GDT_start ; length of GDT
    dd GDT_start ; pointer to structure
DISK:
    db 0x0
GDT_CODE_SEGMENT equ GDT_Code_Section - GDT_start ; offset to code section
GDT_DATA_SEGMENT equ GDT_Data_Section - GDT_start ; offset to data section

times 510 - ($-$$) db 0 ; zeroing out unused parts of sector 512 * sector number - 2 for bootsector number
dw 0xaa55 ;magic number for boot loader

disk_sector: ; data read from disk is placed here, currently 12 sectors are loaded

bits 32
hello: db "Bootloader loading OS....",0

protected_mode:

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
    mov esp,KERNEL_STACK ;point stack pointer to the allocated kernel stack
	extern kernel_main
	call kernel_main ; call the C code
    cli
    hlt

section .bss ; data section of unitilized data, data that dosent need to be taken from the disk
align 4 ; align the data to be 4 byte / 32 bit

KERNEL_STACK_BOTTOM: equ $
	resb 163840 ; Reserve 16 KB for the stack
KERNEL_STACK:
