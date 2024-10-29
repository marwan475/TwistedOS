bits 16

section .entry

extern __bss_start
extern __end

extern kernel_main
global entry

entry:
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

    mov ax, 0x2401
	int 0x15 ; A20 gate

    ; load GDT
    lgdt [GDTD]           

    ; senable protected mode
    mov eax, cr0
    or al, 1
    mov cr0, eax

    
    jmp dword 08h:protected_mode

protected_mode:
    [bits 32]
    
    ;segment registers
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
   
    ; clear bss 
    mov edi, __bss_start
    mov ecx, __end
    sub ecx, edi
    mov al, 0
    cld
    rep stosb

    ; expect boot drive in dl, send it as argument to cstart function
    xor edx, edx
    mov dl, [DRIVE]
    push edx

    call kernel_main

    cli
    hlt

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



