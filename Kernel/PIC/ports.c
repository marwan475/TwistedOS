#include "../headers/ports.h"

// Hardware / port comunication 

void write8bitport(uint16 portnumber,uint8 data){
    // outb is an instruction that sends a byte to a specified port
    __asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (portnumber));
}

uint8 read8bitport(uint16 portnumber){
    uint8 result;
    // inb is an instruction that reads a byte from a specified port
    __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (portnumber));
    return result;
}

void write8bitportSlow(uint16 portnumber,uint8 data){
    // outb is an instruction that sends a byte to a specified port
    __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (data), "Nd" (portnumber));
}

void write16bitport(uint16 portnumber,uint16 data){
    // outw is an instruction that sends a word to a specified port
    __asm__ volatile("outw %0, %1" : : "a" (data), "Nd" (portnumber));
}

uint8 read16bitport(uint16 portnumber){
    uint16 result;
    // inw is an instruction that reads a word from a specified port
    __asm__ volatile("inw %1, %0" : "=a" (result) : "Nd" (portnumber));
    return result;
}

void write16bitportSlow(uint16 portnumber,uint16 data){
    // outw is an instruction that sends a word to a specified port
    __asm__ volatile("outw %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (data), "Nd" (portnumber));
}

void write32bitport(uint16 portnumber,uint32 data){
    // outl is an instruction that sends a long to a specified port
    __asm__ volatile("outl %0, %1" : : "a" (data), "Nd" (portnumber));
}

uint8 read32bitport(uint16 portnumber){
    uint32 result;
    // inl is an instruction that reads a long from a specified port
    __asm__ volatile("inl %1, %0" : "=a" (result) : "Nd" (portnumber));
    return result;
}

void write32bitportSlow(uint16 portnumber,uint32 data){
    // outl is an instruction that sends a long to a specified port
    __asm__ volatile("outl %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (data), "Nd" (portnumber));
} 