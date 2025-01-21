# TwistedOS

x86 32 bit OS from scratch

## Design

#### 2 stage boot loader
- stage 1 loads the FAT 12 table then loads in stage 2 of the bootloader
- stage 2 loads the entire kernel from the FAT 12 file system at above 1mb of memory
  - Initializes GDT
  - Switches to 32 bit mode
  - Jumps execution to kernel code
- 2 stages are used due to the 512 byte limit of the inital bootloader and the 1mb limit on 20bit addressing (16 bit + A gate)

#### Kernel
- installs TSS to allow for interupt handling in user mode
- Set up IDT for interrupt handling
- Initalizes IDT with ISRs
- remaps the PIC to allow for interrupts
- Sets up keyboard driver
- enables interrupts
- Initalizes physical memory manager
  - bit map that manges the workable memory range using blocks of 4096 (size of pages)
- Initalizes virtual memory
  - sets up page table dir and page tables
  - identiy maps first 16mb of memory
  - allocates space for pages using physical memory manager
- Initializes network card driver for rtl8139
  - card sends interrupt any time a packet is recv or sent
- ethernet layer
 - checks if packet is an ARP request or IPv4
- Console support
  - console printing support for output and debugging
  - user is able to enter commands
  - user can enumerate PCI devices
  - user can clear screen

## Resources

https://www.ctyme.com/intr/rb-0106.htm 
https://www.ctyme.com/intr/rb-0607.htm // interrupt codes
https://www.eit.lth.se/fileadmin/eit/courses/eitn50/Literature/fat12_description.pdf // FAT12
https://wiki.osdev.org/Rolling_Your_Own_Bootloader
https://wiki.osdev.org/Creating_an_Operating_System
https://www.sqlpassion.at/archive/2022/03/03/reading-files-from-a-fat12-partition/
https://www.calculator.net/hex-calculator.html?number1=7E00&c2op=%2B&number2=2800&calctype=op&x=Calculate
https://wiki.osdev.org/Memory_Map_(x86)
http://www.brokenthorn.com/Resources/OSDev18.html
https://www.rapidtables.com/convert/number/hex-to-binary.html?x=D
https://wiki.osdev.org/PCI
https://wiki.osdev.org/RTL8139
http://realtek.info/pdf/rtl8139cp.pdf
https://ics.uci.edu/~harris/ics216/pci/PCI_22.pdf
https://github.com/dreamportdev/Osdev-Notes/tree/master/06_Userspace
https://en.wikipedia.org/wiki/Ethernet_frame
https://en.wikipedia.org/wiki/Address_Resolution_Protocol
