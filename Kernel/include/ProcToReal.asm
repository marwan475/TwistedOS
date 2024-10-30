

; Assumes already in 32-bit protected mode with GDT loaded

global biosFunc
bioFunc:
    jmp switch_to_real_mode

switch_to_real_mode:
    pusha ; push all the use registers

    xor eax, eax ; push data segment
    mov ax, ds
    push eax

    mov ax, 0x10 ; use kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp ; pass stack pointer to C function


    ; Switch to real mode
    cli                     ; Disable interrupts
    mov eax, cr0
    and eax, 0xFFFFFFFE     ; Clear PE bit (Protection Enable) in CR0
    mov cr0, eax            ; Write back to CR0 to switch to real mode

    jmp 0x18:real_mode_entry ; Far jump to switch to 16-bit real mode

real_mode_entry:
    ; Set up a 16-bit compatible stack
    mov ax, 0x10            ; Load 16-bit data segment selector
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00          ; Set stack pointer for real mode stack (or another suitable address)

    ; Now in 16-bit real mode, execute any necessary 16-bit code here

    ; ... 16-bit real mode operations

    ; Call to switch back to protected mode
    call switch_to_protected_mode
    ret

switch_to_protected_mode:
    cli                     ; Disable interrupts

    ; Set PE bit to enable protected mode
    mov eax, cr0
    or eax, 0x1             ; Set PE bit (Protection Enable) in CR0
    mov cr0, eax            ; Write back to CR0 to re-enable protected mode

    ; Far jump to flush prefetch and return to 32-bit protected mode
    jmp 0x08:protected_mode_entry

protected_mode_entry:
    add esp, 4

    ; restore old registers and segments
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa

    ; Restore stack pointer and segment registers
    sti                     ; Re-enable interrupts
    ret
