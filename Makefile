
all: img

run: 
	qemu-system-i386 -drive file=TwistedOS.img,format=raw,if=floppy

img: Bootloader/boot.bin OS
	dd if=/dev/zero of=TwistedOS.img bs=512 count=2880
	mkfs.fat -F 12 -n "NBOS" TwistedOS.img
	dd if=Bootloader/boot.bin of=TwistedOS.img conv=notrunc
	mcopy -i TwistedOS.img kernel.bin "::kernel.bin"


Bootloader/boot.bin: Bootloader/BootloaderSTAGE1.asm
	nasm -f bin Bootloader/BootloaderSTAGE1.asm -o Bootloader/boot.bin

Bootloader/boot.o: Bootloader/BootloaderSTAGE2.asm
	nasm -f elf Bootloader/BootloaderSTAGE2.asm -o Bootloader/boot.o

Kernel/include/BiosFunc.o: Kernel/include/BiosFunc.asm
	nasm -f elf Kernel/include/BiosFunc.asm -o Kernel/include/BiosFunc.o

Kernel/IDT/ISR.o: Kernel/IDT/ISR.asm
	nasm -f elf Kernel/IDT/ISR.asm -o Kernel/IDT/ISR.o

Kernel/kernel.o: Kernel/kernel.c
	i386-elf-gcc -m32 -c Kernel/kernel.c -o Kernel/kernel.o -nostdlib -ffreestanding

Kernel/include/utility.o: Kernel/include/utility.c
	i386-elf-gcc -m32 -c Kernel/include/utility.c -o Kernel/include/utility.o -nostdlib -ffreestanding

Kernel/include/ports.o: Kernel/include/ports.c
	i386-elf-gcc -m32 -c Kernel/include/ports.c -o Kernel/include/ports.o -nostdlib -ffreestanding

Kernel/keyboard/keyboardDriver.o: Kernel/keyboard/keyboardDriver.c
	i386-elf-gcc -m32 -c Kernel/keyboard/keyboardDriver.c -o Kernel/keyboard/keyboardDriver.o -nostdlib -ffreestanding

Kernel/memory/memory.o: Kernel/memory/memory.c
	i386-elf-gcc -m32 -c Kernel/memory/memory.c -o Kernel/memory/memory.o -nostdlib -ffreestanding

OS: Kernel/kernel.o Kernel/include/utility.o Kernel/include/ports.o Kernel/IDT/ISR.o Bootloader/boot.o Kernel/keyboard/keyboardDriver.o Kernel/memory/memory.o Kernel/include/BiosFunc.o
	i386-elf-gcc -m32 -nostdlib -ffreestanding Bootloader/boot.o Kernel/kernel.o Kernel/include/utility.o Kernel/include/BiosFunc.o Kernel/include/ports.o Kernel/IDT/ISR.o Kernel/keyboard/keyboardDriver.o Kernel/memory/memory.o -o kernel.bin -T Linker/linker.ld

clean:
	rm TwistedOS.img -f
	rm Bootloader/boot.bin -f
	rm Bootloader/boot.o -f
	rm Kernel/*.o -f
	rm Kernel/include/*.o -f
	rm Kernel/IDT/*.o -f
	rm Kernel/keyboard/*.o -f
	rm Kernel/memory/*.o -f
	rm kernel.bin -f
