#include "headers/utility.h"
#include "headers/kernel.h"
#include "IDT/ISR.h"
#include "PIC/PIC.h"

void kernel_main()
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


    kernelprint("Welcome to Twisted OS V%d %n",1);

    while(1){
        
    }

    
}
