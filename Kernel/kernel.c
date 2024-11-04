#include "headers/utility.h"
#include "headers/kernel.h"
#include "IDT/ISR.h"
#include "PIC/PIC.h"
#include "memory/physicalmemory.h"
#include "memory/virtualmemory.h"

// Start: 0x00100000 | End: 0x00FFFFFF | Size: 0x00F00000 (15 MiB) | RAM -- free for use

uint8 kernel_cs = 0x08 | 0;
uint8 kernel_ss = 0x10 | 0;
uint8 user_cs = 0x28 | 3; // 0x2B
uint8 user_ss = 0x30 | 3; //0x33

__attribute__((naked, noreturn))
void switch_to_user_mode(uint32 stack_addr, uint32 code_addr)
{
    asm volatile(" \
        push $0x33 \n\
        push %0 \n\
        push $0x202 \n\
        push $0x2B \n\
        push %1 \n\
        iret \n\
        " :: "r"(stack_addr), "r"(code_addr));
}

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
