
all: OS

run: 
	qemu-system-i386 -drive file=TwistedOS.img,format=raw

img: 
	dd if=/dev/zero of=TwistedOS.img bs=512 count=2880
	dd if=kernel.bin of=TwistedOS.img conv=notrunc,fsync


Bootloader/boot.o: Bootloader/Bootloader.asm
	nasm -f elf Bootloader/Bootloader.asm -o Bootloader/boot.o

Kernel/IDT/ISR.o: Kernel/IDT/ISR.asm
	nasm -f elf Kernel/IDT/ISR.asm -o Kernel/IDT/ISR.o

Kernel/kernel.o: Kernel/kernel.c
	i386-elf-gcc -m32 -c Kernel/kernel.c -o Kernel/kernel.o -nostdlib -ffreestanding

Kernel/include/utility.o: Kernel/include/utility.c
	i386-elf-gcc -m32 -c Kernel/include/utility.c -o Kernel/include/utility.o -nostdlib -ffreestanding

Kernel/include/ports.o: Kernel/include/ports.c
	i386-elf-gcc -m32 -c Kernel/include/ports.c -o Kernel/include/ports.o -nostdlib -ffreestanding

OS: Bootloader/boot.o Kernel/kernel.o Kernel/include/utility.o Kernel/include/ports.o Kernel/IDT/ISR.o
	i386-elf-gcc -m32 -nostdlib -ffreestanding Bootloader/boot.o Kernel/kernel.o Kernel/include/utility.o Kernel/include/ports.o Kernel/IDT/ISR.o -o kernel.bin -T Linker/linker.ld

clean:
	rm TwistedOS.img -f
	rm Bootloader/*.o -f
	rm Kernel/*.o -f
	rm Kernel/include/*.o -f
	rm Kernel/IDT/*.o -f
	rm kernel.bin -f
