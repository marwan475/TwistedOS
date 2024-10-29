
bits 16

global start
start:

    mov ax, 0x2401
    int 0x15 ; enable A20

    mov ax, 0x3
    int 0x10 ; set vga text mode 

    jmp disk_read_error

disk_read_error:
  mov si,errormsg ; point si register to hello label memory location
  mov ah,0x0e  
  .loop:
    lodsb
    or al,al  
    jz $   
    int 0x10 ; runs BIOS interrupt 0x10 - Video Services
    jmp .loop

.halt:
    cli
    hlt

errormsg: db "TEST",0   

