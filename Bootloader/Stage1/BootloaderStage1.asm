;x86 Memory Map RAM
;0x00000000 - 0x000003FF - Real Mode Interrupt Vector Table
;0x00000400 - 0x000004FF - BIOS Data Area
;0x00000500 - 0x00007BFF - Unused
;0x00007C00 - 0x00007DFF - Boot Sector
;0x00007E00 - 0x0009FFFF - Unused
;0x000A0000 - 0x000BFFFF - Video RAM (VRAM) Memory
;0x000B0000 - 0x000B7777 - Monochrome Video Memory
;0x000B8000 - 0x000BFFFF - Color Video Memory
;0x000C0000 - 0x000C7FFF - Video ROM BIOS
;0x000C8000 - 0x000EFFFF - BIOS Shadow Area
;0x000F0000 - 0x000FFFFF - System BIOS

; Bios loads bootloader into the bootsector at 0x7C00
org 0x7C00

;Bios starts in 16 bit Real Mode
bits 16

; FAT 12 Disk Structure

;       DISK      | Logical Block Address
;  _______________ ______________________
; |  Boot Sector  |           0          |
; |  FAT Table 1  |           1          |
; |  FAT Table 2  |           10         |
; |  Root Dir     |           19         |
; |  Data Area    |           33         |
;  --------------- ----------------------

; FAT 12 headers (Bios Parameters | Layout of FAT filesystem)

jmp short start ; IGNORE
nop

oem: db "Twisted " ; 8 bytes
bytespersector: dw 512 ; bytes per sector 
sectorspercluster: db 1 ; sectors per cluster
reservedsectors: dw 1 ; number of reserved sectors
numberofFATs: db 2 ; number of FAT tables
rootentries: dw 224 ; number of entries in the root dir
totalsectors: dw 2880 ; number of sectors on floppy
media: db 0xF0 ; Ignore
sectorsperFAT: dw 9 ; sectors per FAT table (size of FAT table)
sectorspertrack: dw 18 ; sectors per track
headspercylinder: dw 2 ; heads per cylinder
hiddensectors: dd 0 ; Ignore
totalbigsectors: dd 0 ; total sector count for FAT32
drivenumber: db 0 ; Drive Number
unused: db 0 ; Ignore
bootsignature: db 0x29 ; Boot Signature
serialnumber: dd 0xb3b4b5b6 ; disk serial number
volumelabel: db "TWISTED  OS" ; 11 bytes
filesystem: db "FAT12   " ; 8 bytes 



start:
    jmp bootloader

; BIOS only loads first 512 bytes at sector 0 on the disk | GOAL : load Stage 2 of the bootloader from disk that contains the rest of the code
bootloader:

    ; Setting segment registers to 0x0000 (thats where we are)
    
    mov ax, 0x0000
    mov ds, ax ; points to data segment
    mov es, ax ; extra
    mov fs, ax ; extra
    mov gs, ax ; extra
    mov ss, ax ; points to stack segment

    ; setitng stack pointer below the boot sector in RAM as that are is unused for now (Stack grows downwards)
    mov sp, 0x7C00

    mov [drivenumber], dl ; save drive number

    ; We will load FAT table 1 at 0x7E00 unused section after boot loader
    ; Load FAT table 1
    mov ax, 1 ; LBA of FAT table 1
    mov cl, 1 ; number of sectors
    mov bx, 0x7E00 ; where to load in RAM
    call ReadDisk

; finds the LBA of root dir
; LBA = ( bootsector + numFATtables ) * sectors per FAT + reserved sectors
findLBA_root_dir:
    
    ; find LBA of root dir
    mov ax, [sectorsperFAT]
    mov bl, [numberofFATs]
    xor bh, bh
    mul bx
    add ax, [reservedsectors]
    push ax

; finds the size of the root dir 
; size = (32 * number of entries) bytes per sector
; each entry is 32 bytes
size_root_dir:

    mov ax, [sectorsperFAT]
    shl ax, 5
    xor dx, dx
    div word [bytespersector]

    test dx,dx
    jz read_root
    inc ax

; we now have size and LBA of root dir
; read the root dir into memory
read_root:

    mov cl, al
    pop ax
    mov dl, [drivenumber]
    mov bx, root_dir
    call ReadDisk

; we have the root dir loaded into memory
; we now have to search for our file
file_search:

    xor bx, bx
    mov di, root_dir

; goes throuh root dir and searches for entry that matchs our file name    
.search:

    mov  si, stage2
    mov cx, 11
    push di
    repe cmpsb ; comapres bytes at ds:si and es:di | repear while equal cx times
    pop di
    je file_found

    ; move to next file, if all entries checked jmp to err
    add di, 32
    inc bx
    cmp bx, [rootentries]
    jl .search

    jmp search_error

; load file from disk into memory
; di contains pointer to entrey for file
; offset 26 contains the first logical cluster of the file
file_found:
    mov ax, [di + 26]
    mov [logicalcluster], ax

    ; load full FAT table
    mov ax, 1 ; LBA of first FAT table
    mov bx, root_dir
    mov cl, [sectorsperFAT]
    mov dl, [drivenumber]
    call ReadDisk

    ; where we will load our file, unused 0000:0500
    mov ax, 0x0000
    mov es, ax
    mov bx, 0x500

; File location = FAT entry number - 2 + 33 (Data region for FAt12) 
.loadfile:
    mov ax, [logicalcluster]
    add ax, 31 ; 33 - 2 = 31

    ; load one cluster of file
    mov cl, byte [sectorspercluster]
    mov dl ,[drivenumber]
    call ReadDisk

    ; move to next cluster
    ; incrment load loaction
    mov ax, es
    add ax, 0x0020
    mov es, ax

    ; find next cluster
    mov ax, [logicalcluster]
    mov cx, 3
    mul cx
    mov cx, 2
    div cx

    mov si, root_dir
    add si, ax
    mov ax, [ds:si]

    or dx, dx
    jz .even

.odd:
    shr ax, 4
    jz .nextcluster

.even:
    and ax, 0x0FFF

; checking if at end of file or next cluster for file
.nextcluster:
    cmp ax, 0x0FF8 
    jae file_loaded

    mov [logicalcluster], ax
    jmp .loadfile

file_loaded:
    mov di, [drivenumber]
    
    mov ax, 0x0000
    mov ds, ax
    mov es, ax

    jmp 0x0000:0x0500

    cli
    hlt

;convert LBA to CHS
;ax = LBA
;stores result in registers for diskread
LBAtoCHS:

    push ax
    push dx

    xor dx, dx ; set dx to 0
    div word [sectorspertrack] ; LBA / sectors per track sets awnser in ax and puts remainder in dx
    inc dx
    mov cx, dx ; sector 

    xor dx, dx
    div word [headspercylinder] ; divide running calc by heads per cyl
    mov dh, dl ; head

    mov ch, al ; cyl lower 8 bits
    shl ah, 6
    or cl, ah ; cylinder

    pop ax
    mov dl, al
    pop ax
    ret

;Reads sectors from lba an stores them in RAM
;ax = LBA
;es:bx = location
;cl = number of sectors
ReadDisk:
    
    push ax
    push bx
    push cx
    push dx
    push di

    push cx ; save number of sectors
    call LBAtoCHS
    pop ax

    ; int 13h interupt used to read from disk registers are already set from past function call
    mov ah, 0x02
    pusha
    stc
    int 0x13

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
  mov si,errormsg 
  jmp print_error

errorms: db "kernel not found",0    

search_error:
  mov si,errorms 
  jmp print_error

print_error:
  mov ah,0x0e  
  .loop:
    lodsb
    or al,al  
    jz $   
    int 0x10 ; runs BIOS interrupt 0x10 - Video Services
    jmp .loop

stage2: db "STAGE2  BIN"
logicalcluster: dw 0

times 510 - ($-$$) db 0 ; padding unused parts of the bootsector
dw 0xaa55 ; Bios needs this to be the last 2 bytes of bootsector

; buffer to store root dir in    
root_dir:


