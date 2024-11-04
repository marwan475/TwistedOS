
all: img

run: 
	qemu-system-i386 -m 128M -drive file=TwistedOS.img,format=raw,if=floppy -device rtl8139,netdev=net0 -netdev user,id=net0

img: Stage1 Stage2 OS
	dd if=/dev/zero of=TwistedOS.img bs=512 count=2880
	mkfs.fat -F 12 -n "NBOS" TwistedOS.img
	dd if=boot.bin of=TwistedOS.img conv=notrunc
	mcopy -i TwistedOS.img stage2.bin "::stage2.bin"
	mcopy -i TwistedOS.img kernel.bin "::kernel.bin"


Stage1: Bootloader/Stage1/BootloaderStage1.asm
	nasm -f bin Bootloader/Stage1/BootloaderStage1.asm -o boot.bin

Bootloader/Stage2/boot.o: Bootloader/Stage2/BootloaderSTAGE2.asm
	nasm -f elf Bootloader/Stage2/BootloaderSTAGE2.asm -o Bootloader/Stage2/boot.o

Bootloader/Stage2/disk.o: Bootloader/Stage2/Disk.asm
	nasm -f elf Bootloader/Stage2/Disk.asm -o Bootloader/Stage2/disk.o

Bootloader/Stage2/stage2.o: Bootloader/Stage2/stage2.c
	i386-elf-gcc -m32 -c Bootloader/Stage2/stage2.c -o Bootloader/Stage2/stage2.o -nostdlib -ffreestanding

Stage2: Bootloader/Stage2/boot.o Bootloader/Stage2/stage2.o Kernel/Terminal/Terminal.o Bootloader/Stage2/disk.o
	i386-elf-gcc -m32 -nostdlib -ffreestanding Kernel/Terminal/Terminal.o Bootloader/Stage2/stage2.o Bootloader/Stage2/boot.o Bootloader/Stage2/disk.o -o stage2.bin -T Linker/bootlinker.ld

Kernel/IDT/ISR.o: Kernel/IDT/ISR.asm
	nasm -f elf Kernel/IDT/ISR.asm -o Kernel/IDT/ISR.o

Kernel/kernel.o: Kernel/kernel.c
	i386-elf-gcc -m32 -c Kernel/kernel.c -o Kernel/kernel.o -nostdlib -ffreestanding

Kernel/Terminal/Terminal.o: Kernel/Terminal/Terminal.c
	i386-elf-gcc -m32 -c Kernel/Terminal/Terminal.c -o Kernel/Terminal/Terminal.o -nostdlib -ffreestanding

Kernel/PIC/ports.o: Kernel/PIC/ports.c
	i386-elf-gcc -m32 -c Kernel/PIC/ports.c -o Kernel/PIC/ports.o -nostdlib -ffreestanding

Kernel/keyboard/keyboardDriver.o: Kernel/keyboard/keyboardDriver.c
	i386-elf-gcc -m32 -c Kernel/keyboard/keyboardDriver.c -o Kernel/keyboard/keyboardDriver.o -nostdlib -ffreestanding

Kernel/memory/physicalmemory.o: Kernel/memory/physicalmemory.c
	i386-elf-gcc -m32 -c Kernel/memory/physicalmemory.c -o Kernel/memory/physicalmemory.o -nostdlib -ffreestanding

Kernel/memory/virtualmemory.o: Kernel/memory/virtualmemory.c
	i386-elf-gcc -m32 -c Kernel/memory/virtualmemory.c -o Kernel/memory/virtualmemory.o -nostdlib -ffreestanding

OS: Kernel/kernel.o Kernel/Terminal/Terminal.o Kernel/PIC/ports.o Kernel/IDT/ISR.o Kernel/keyboard/keyboardDriver.o Kernel/memory/physicalmemory.o Kernel/memory/virtualmemory.o 
	i386-elf-gcc -m32 -nostdlib -ffreestanding Kernel/kernel.o Kernel/Terminal/Terminal.o Kernel/PIC/ports.o Kernel/IDT/ISR.o Kernel/keyboard/keyboardDriver.o Kernel/memory/physicalmemory.o Kernel/memory/virtualmemory.o -o kernel.bin -T Linker/kernellinker.ld

clean:
	rm TwistedOS.img -f
	rm boot.bin -f
	rm Bootloader/Stage2/*.o -f
	rm Kernel/*.o -f
	rm Kernel/PIC/*.o -f
	rm Kernel/Terminal/*.o -f
	rm Kernel/IDT/*.o -f
	rm Kernel/keyboard/*.o -f
	rm Kernel/memory/*.o -f
	rm kernel.bin -f
	rm stage2.bin -f
