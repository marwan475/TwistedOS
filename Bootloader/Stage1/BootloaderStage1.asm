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
sectorsperfat: dw 9 ; sectors per FAT table (size of FAT table)
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

; where we will store our CHS address after converting from LBA
cursector db 0x00
curhead db 0x00
curtrack db 0x00

times 510 - ($-$$) db 0 ; padding unused parts of the bootsector
dw 0xaa55 ; Bios needs this to be the last 2 bytes of bootsector




