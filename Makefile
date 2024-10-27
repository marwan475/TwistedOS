
all: OS

run: 
	qemu-system-i386 -cdrom TwistedOS.iso

iso: 
	xorriso -as mkisofs -b kernel.bin -no-emul-boot -boot-load-size 4 -o TwistedOS.iso .

Bootloader/boot.o: Bootloader/Bootloader.asm
	nasm -f elf Bootloader/Bootloader.asm -o Bootloader/boot.o

Kernel/kernel.o: Kernel/kernel.c
	i386-elf-gcc -m32 -c Kernel/kernel.c -o Kernel/kernel.o -nostdlib -ffreestanding

Kernel/include/utility.o: Kernel/include/utility.c
	i386-elf-gcc -m32 -c Kernel/include/utility.c -o Kernel/include/utility.o -nostdlib -ffreestanding

OS: Bootloader/boot.o Kernel/kernel.o Kernel/include/utility.o
	i386-elf-gcc -m32 -nostdlib -ffreestanding Bootloader/boot.o Kernel/kernel.o Kernel/include/utility.o -o kernel.bin -T Linker/linker.ld

clean:
	rm TwistedOS.iso -f
	rm Bootloader/*.o -f
	rm Kernel/*.o -f
	rm Kernel/include/*.o -f
	rm kernel.bin -f