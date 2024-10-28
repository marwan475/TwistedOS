#include "headers/utility.h"
#include "headers/kernel.h"
#include "IDT/ISR.h"

void kernel_main()
{
    kernelprint("IDT intialized",0,1);
    kernelprint("Welcome to Twisted OS%nThis is Version:%d!",0,2,1);   
}
