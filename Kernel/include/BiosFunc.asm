
global biosFunc

biosFunc:
    [bits 32]
    push ebp
    mov ebp,esp

    
    jmp word 0x18:.protectedmode16

.protectedmode16:
    [bits 16]

    ; disable protected mode
    mov eax, cr0
    mov al, ~1
    mov cr0, eax

    jmp word 0x00:.realmode

.realmode:
    
    mov ax, 0
    mov ds, ax
    mov ss, ax

    sti

    push es
    push bx
    push esi
    push di

    ; bios function here

    pop di
    pop esi
    pop bx
    pop es

    push eax
    jmp protectedmode

protectedmode:
    [bits 16]
    
    cli

    ; set protected mode
    mov eax, cr0
    or al, 1
    mov cr0, eax

    jmp dword 0x08:.bit32mode

.bit32mode:
    [bits 32]

    mov ax, 0x10
    mov ds, ax
    mov ss, ax

    pop eax

    mov esp,ebp
    pop ebp
    ret

