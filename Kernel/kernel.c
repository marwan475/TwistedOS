#include "headers/utility.h"
#include "headers/kernel.h"
#include "IDT/ISR.h"
#include "PIC/PIC.h"
#include "memory/memory.h"
#include "headers/BiosFunctions.h"

void kernel_main()
{
    kernelprint("Global Discriptor Table loaded%n");

    initIDT();
    kernelprint("Interupt Discriptor Table intialized%n");

    biosFunc();

    ISR_init();
    kernelprint("Interupt Service Routines intialized%n");

    PICremap();
    kernelprint("PIC remaped%n");

    keyboardDriver();
    kernelprint("keyboard Driver loaded%n");

    enableinterrupts();
    kernelprint("interrupts enabled%n");

    heapinit();
    kernelprint("Bitmap initialized%n");

    kernelprint("Welcome to Twisted OS V%n"); 

    while(1){
      console();   
    }

    
}
