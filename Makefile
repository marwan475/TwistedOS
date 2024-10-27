
all: OS

run: 
	qemu-system-i386 -cdrom TwistedOS.iso

iso: 
	xorriso -as mkisofs -b kernel.bin -no-emul-boot -boot-load-size 4 -o TwistedOS.iso kernel.bin


Bootloader/boot.o: Bootloader/Bootloader.asm
	nasm -f elf Bootloader/Bootloader.asm -o Bootloader/boot.o

Kernel/kernel.o: Kernel/kernel.c
	i386-elf-gcc -m32 -c Kernel/kernel.c -o Kernel/kernel.o -nostdlib -ffreestanding

Kernel/include/utility.o: Kernel/include/utility.c
	i386-elf-gcc -m32 -c Kernel/include/utility.c -o Kernel/include/utility.o -nostdlib -ffreestanding

Kernel/include/ports.o: Kernel/include/ports.c
	i386-elf-gcc -m32 -c Kernel/include/ports.c -o Kernel/include/ports.o -nostdlib -ffreestanding

OS: Bootloader/boot.o Kernel/kernel.o Kernel/include/utility.o Kernel/include/ports.o
	i386-elf-gcc -m32 -nostdlib -ffreestanding Bootloader/boot.o Kernel/kernel.o Kernel/include/utility.o Kernel/include/ports.o -o kernel.bin -T Linker/linker.ld

clean:
	rm TwistedOS.iso -f
	rm Bootloader/*.o -f
	rm Kernel/*.o -f
	rm Kernel/include/*.o -f
	rm kernel.bin -f