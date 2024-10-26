
all: iso

iso: boot.bin
	xorriso -as mkisofs -b boot.bin -no-emul-boot -boot-load-size 4 -o TwistedOS.iso Bootloader/bootiso/

boot.bin: Bootloader/32Bootloader.asm
	nasm -f bin Bootloader/32Bootloader.asm -o Bootloader/bootiso/boot.bin

clean:
	rm TwistedOS.iso
	rm Bootloader/bootiso/boot.bin