bits 16

section .entry

extern __bss_start
extern __end

LBA: dw 0
kernel: db "KERNEL  BIN"
rootentries: dw 224

; Root dir loaded at 0x7E00
; FAT Table loaded at 0xA6FF
global entry

entry:

file_search:
    xor bx, bx
    mov di, 0x7E00
.search:
    mov si, kernel
    mov cx, 11
    push di
    repe cmpsb
    pop di
    je file_found

    add di, 32
    inc bx
    cmp bx, [rootentries]
    jl .search

    jmp search_error
file_found:
    mov ax, [di + 26]
    add ax, 31
    mov [LBA], ax

    cli

    ; save boot drive
    mov [DRIVE], dl

    ; setup stack
    mov ax, ds
    mov ss, ax
    mov sp, 0xFFF0
    mov bp, sp

    mov ax, 0x3
    int 0x10 ; set vga txt mode 3

    ; disable cursor
    mov ah, 0x01
    mov ch, 0x3F
    int 0x10

    mov ax, 0x2401
    int 0x15 ; A20 gate
    
    ; load GDT
    lgdt [GDTD]           

    ; enable protected mode
    mov eax, cr0
    or al, 1
    mov cr0, eax

    
    jmp dword 08h:protected_mode

; entering protected mode
protected_mode:
    [bits 32]
    
    ;segment registers
    mov ax, 0x10
    mov ds, ax
    mov ss, ax

    ; hand control to kernel
    mov esp,0xFFFF

    movzx eax, word [LBA]
    push eax

    extern stage2
    call stage2

    cli
    hlt

errorms: db "kernel not found",0    

search_error:
  mov si,errorms ; point si register to hello label memory location
  jmp print_error

print_error:
  mov ah,0x0e  
  .loop:
    lodsb
    or al,al  
    jz $   
    int 0x10 ; runs BIOS interrupt 0x10 - Video Services
    jmp .loop

; Global Discriptor Table
GDT:    
	dq 0
	dw 0FFFFh
	dw 0
	db 0
	db 10011010b
	db 11001111b
	db 0
	dw 0FFFFh
	dw 0
	db 0
	db 10010010b
	db 11001111b
	db 0
	dw 0FFFFh
	dw 0
	db 0
	db 10011010b
	db 00001111b
	db 0
	dw 0FFFFh
	dw 0
	db 0
	db 10010010b
	db 00001111b
	db 0
GDTD: dw GDTD - GDT - 1
	dd GDT

DRIVE: db 0



