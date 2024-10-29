#include "headers/utility.h"
#include "headers/kernel.h"
#include "IDT/ISR.h"
#include "PIC/PIC.h"

void kernel_main()
{
    initIDT();
    kernelprint("Interupt Discriptor Table intialized",0,1);

    ISR_init();
    kernelprint("Interupt Service Routines intialized",0,2);

    PICremap();
    kernelprint("PIC remaped",0,3);

    keyboardDriver();
    kernelprint("keyboard Driver loaded",0,4);

    enableinterrupts();
    kernelprint("interrupts enabled",0,5);


    kernelprint("Welcome to Twisted OS",0,6);

    while(1){
        
    }

    
}
