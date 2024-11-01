
%macro Enter16bitmode 0
    [bits 32]
    jmp word 0x18:.protectedmode16bit

.protectedmode16bit:
    [bits 16]

    ; disable protected mode
    mov eax, cr0
    and al, ~1
    mov cr0, eax

    jmp word 0x00:.realmode

.realmode:

    ;set up segments
    mov ax, 0x0000
    mov ds, ax
    mov ss, ax

    ;renable interrupts
    sti

%endmacro

%macro Enter32bitmode 0
    ;disable interrupts
    cli

    ; enable protected mode
    mov eax,cr0
    or al, 1
    mov cr0, eax

    jmp dword 0x08:.protectedmode

.protectedmode:
    [bits 32]

    mov ax, 0x10
    mov ds, ax
    mov ss, ax

%endmacro

%macro CAddrtox86Addr 4
    mov %3, %1
    shr %3, 4
    mov %2, %4
    mov %3, %1
    and %3, 0xF

%endmacro

; reads disk into buffer and returns success or fail    
global ReadDisk
ReadDisk:

    push ebp
    mov ebp, esp

    Enter16bitmode ; enter 16 bit mode to use bios functions

    push ebx
    push es

    mov dl, [bp + 8] ; drive
    mov ch, [bp +12] ; cylinder
    mov cl, [bp +13]
    shl cl, 6

    mov al, [bp + 16] ; sector
    and al, 0x3F
    or cl, al

    mov dh, [bp + 20] ; head

    mov al, [bp + 24] ; sector count

    CAddrtox86Addr [bp + 28], es, ebx, bx

    ; read from disk
    mov ah, 0x02
    stc
    int 0x13

    mov eax, 1 ; succes
    sbb eax, 0 ; fail

    pop es
    pop ebx

    push eax

    Enter32bitmode

    pop eax

    mov esp, ebp
    pop ebp
    ret

global FindNextCluster    
FindNextCluster:
    push ebp
    mov ebp, esp

    mov ax, [bp + 8]
    sub ax, 31
    mov cx, 3
    mul cx
    mov cx, 2
    div cx

    mov si, 0xA6FF
    add si, ax
    mov ax, [ds:si] 

    or dx, dx
    jz .even

.odd:
    shr ax, 4
    jz .nextcluster

.even:
    and ax, 0x0FFF

.nextcluster:
    
    cmp ax, 0x0FF8
    jae done

    add ax, 31

    mov esp, ebp
    pop ebp

    ret

done:
    mov ax, 0
    
    mov esp, ebp
    pop ebp
    ret
