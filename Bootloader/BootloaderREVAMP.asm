org 0x7C00 ; bios loads at this address, set this as the offset 
bits 16

; FAT 12 header

jmp short start
nop

bdboem: db 'Twisted '
bdbBytesPerSector: dw 512
bdbSectorsPerCluster: db 1
bdbReservedSectors: dw 1
bdbNumberOfFATs: db 2
bdbEntries: dw 0E0h
bdbTotalSectors: dw 2880
bdbMedia: db 0F0h
bdbSectorsPerFAT: dw 9
bdbSectorsPerTrack: dw 18
bdbHeadsPerCylinder: dw 2
bdbHiddenSectors: dd 0
bdbTotalSectorsLarge: dd 0

bsDrivenumber: db 0
bsUnused: db 0
bsExtBootSignature: db 29h
bsSerialNumber: db 10h, 32h, 54h, 74h
bsVolumeLabel: db "TWISTED OS "
bsFileSystem db "FAT12   "

start:
    jmp bootloader

bootloader:

    mov ax, 0x0000
    mov ds, ax ; points to data segment
    mov es, ax ; extra
    mov fs, ax ; extra
    mov gs, ax ; extra
    mov ss, ax ; points to stack segment

    mov sp , 0x7C00 ; setting stack pointer

    mov [bsDrivenumber], dl ; saving drive number

    mov ax, 1 ; LBA 1
    mov cl, 1 ; sectors 1
    mov bx, 0x7E00
    call ReadFromDisk

    call printm

    jmp find_root_dir

; finds the LBA and size of root dir for FAT 12
find_root_dir:

    ; find LBA of root dir = bootsector + fats * sectors per fat
    mov ax, [bdbSectorsPerFAT]
    mov bl, [bdbNumberOfFATs]
    xor bh,bh
    mul bx
    add ax, [bdbReservedSectors]
    push ax 

    ; find size of root dir 32 * num of entries / bytes per sector
    mov ax, [bdbSectorsPerFAT]
    shl ax, 5
    xor dx, dx
    div word [bdbBytesPerSector]

    test dx,dx
    jz read_root
    inc ax

; reads the root dir from disk into memory
read_root:

    mov cl, al ; read sectors = size of root dir
    pop ax ; LBA of root dir
    mov dl, [bsDrivenumber] 
    mov bx, root_dir ; where to store read
    call ReadFromDisk

    jmp kernel_search

; searchs for kernel.bin in root dir
kernel_search:

    xor bx, bx
    mov di, root_dir

.search:
    mov si, kernel_file
    mov cx, 11
    push di
    repe cmpsb; compares bytes at ds:si and es:di | repeat while equal cx times
    pop di
    je kernel_found

    add di, 32
    inc bx
    cmp bx, [bdbEntries]
    jl .search

    jmp kernelL_error


kernel_found:

    cli
    hlt

hello: db "Bootloader...",0    

printm:
  mov si,hello ; point si register to hello label memory location
  mov ah,0x0e  
  .loop:
    lodsb
    or al,al  
    jz endp   
    int 0x10 ; runs BIOS interrupt 0x10 - Video Services
    jmp .loop

endp:
    ret

; Disk functions

; logical block adressing to Cylinder head sector, puts sector head and cyl in req registers
LBAtoCHS:

    push ax
    push dx

    xor dx, dx ; set dx to 0
    div word [bdbSectorsPerTrack] ; LBA / sectors per track sets awnser in ax and puts remainder in dx
    inc dx
    mov cx, dx ; sector 

    xor dx, dx
    div word [bdbHeadsPerCylinder] ; divide running calc by heads per cyl
    mov dh, dl ; head

    mov ch, al ; cyl lower 8 bits
    shl ah, 6
    or cl, ah ; cylinder

    pop ax
    mov dl, al
    pop ax
    ret

; reads sectors from disk, ax contains LBA, es * 16 + bx is where memory will be stored
ReadFromDisk:

    push ax
    push bx
    push cx
    push dx
    push di

    push cx ; save number of sectors
    call LBAtoCHS
    pop ax

    ; int 13h interupt used to read from disk registers are already set from past function call
    mov ah, 02h
    pusha
    stc
    int 13h

    jc disk_read_error

    popa

    pop di
    pop dx
    pop cx
    pop bx
    pop ax

    ret


errormsg: db "disk read error",0    

disk_read_error:
  mov si,errormsg ; point si register to hello label memory location
  mov ah,0x0e  
  .loop:
    lodsb
    or al,al  
    jz $   
    int 0x10 ; runs BIOS interrupt 0x10 - Video Services
    jmp .loop

errorms: db "kernel not found",0    

kernelL_error:
  mov si,errorms ; point si register to hello label memory location
  mov ah,0x0e  
  .loop:
    lodsb
    or al,al  
    jz $   
    int 0x10 ; runs BIOS interrupt 0x10 - Video Services
    jmp .loop

kernel_file: db "KERNEL  BIN"

times 510 - ($-$$) db 0
dw 0xaa55

root_dir: