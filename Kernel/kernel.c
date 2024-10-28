#include "headers/utility.h"
#include "headers/kernel.h"
#include "IDT/ISR.h"

void kernel_main()
{
    initIDT();
    kernelprint("Interupt Discriptor Table intialized",0,1);

    ISR_init();
    kernelprint("Interupt Service Routines intialized",0,2);

    kernelprint("Welcome to Twisted OS",0,3);   
}
