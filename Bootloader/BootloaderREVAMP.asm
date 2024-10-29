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

    hlt

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


times 510 - ($-$$) db 0
dw 0xaa55