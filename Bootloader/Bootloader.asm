bits 16 ; starting in 16 bit mode
org 0x7c00 ; bios loads bootloader to this address, setting offset

mov si, 0 ; counter for print function

print: ; printing function, prints a char using interupt 
    mov ah, 0x0e ; setting interupt code
    mov al, [hello + si] ; getting char from memory
    int 0x10 ; calling interupt
    add si, 1 ; moving to next char, incrementing counter
    cmp byte [hello + si], 0 ; checking if at end of string
    jne print ; loops untill all chars are printed

jmp $ ; infinte loop after code is done

hello: ; string to be printed
    db "Welcome to TwistedOS",0

times 510 - ($-$$) db 0 ; fills rest of code with 0s
dw 0xaa55 ;magic number for boot loader




