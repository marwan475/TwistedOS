
all: iso

run: iso
	qemu-system-i386 -cdrom TwistedOS.iso

iso: OS
	xorriso -as mkisofs -b kernel.bin -no-emul-boot -boot-load-size 4 -o TwistedOS.iso .

Bootloader/boot.o: Bootloader/32Bootloader.asm
	nasm -f elf Bootloader/32Bootloader.asm -o Bootloader/boot.o

Kernel/kernel.o: Kernel/kernel.c
	i386-elf-gcc -m32 -c Kernel/kernel.c -o Kernel/kernel.o -nostdlib -ffreestanding

OS: Bootloader/boot.o Kernel/kernel.o
	i386-elf-gcc -m32 -nostdlib -ffreestanding Bootloader/boot.o Kernel/kernel.o -o kernel.bin -T Linker/linker.ld

clean:
	rm TwistedOS.iso -f
	rm Bootloader/boot.o -f
	rm Kernel/kernel.o -f
	rm kernel.bin -f