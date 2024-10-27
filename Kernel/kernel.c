#include "headers/utility.h"
#include "headers/kernel.h"
#include "headers/ports.h"
#include "IDT/IDT.h"

void kernel_main()
{
    initIDT();
    kernelprint("IDT intialized",0,1);
    kernelprint("Welcome to Twisted OS%nThis is Version:%d!",0,2,1);   
}