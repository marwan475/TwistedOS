#include "headers/utility.h"
#include "headers/kernel.h"
#include "IDT/ISR.h"
#include "PIC/PIC.h"
#include "memory/physicalmemory.h"
#include "memory/virtualmemory.h"

// Start: 0x00100000 | End: 0x00FFFFFF | Size: 0x00F00000 (15 MiB) | RAM -- free for use

void __attribute__((section(".entry"))) kernel_main()
{

    kernelprint("Global Discriptor Table loaded%n");

    initIDT();
    kernelprint("Interupt Discriptor Table intialized%n");

    ISR_init();
    kernelprint("Interupt Service Routines intialized%n");

    PICremap();
    kernelprint("PIC remaped%n");

    keyboardDriver();
    kernelprint("keyboard Driver loaded%n");

    enableinterrupts();
    kernelprint("interrupts enabled%n");

    physicalmemoryinit();
    kernelprint("Physical Memory manager intialized%n");

    virtualinit();
    kernelprint("Virtual Memory init%n");

    kernelprint("Welcome to Twisted OS V%n"); 

    while(1){
      console();   
    }

    
}
