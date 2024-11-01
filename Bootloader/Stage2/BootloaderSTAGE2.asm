bits 16

section .entry

extern __bss_start
extern __end

;extern kernel_main
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
   
    ; clear bss 
    mov edi, __bss_start
    mov ecx, __end
    sub ecx, edi
    mov al, 0
    cld
    rep stosb

    ; hand control to kernel
    mov esp,kernel_stack_top

    extern stage2
    call stage2

    cli
    hlt

errormsg: db "disk read error",0    

disk_read_error:
  mov si,errormsg ; point si register to hello label memory location
  jmp print_error

errorms: db "kernel not found",0    

kernelL_error:
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

section .bss
align 4
kernel_stack_bottom: equ $
	resb 16384 ; 16 KB
kernel_stack_top:


