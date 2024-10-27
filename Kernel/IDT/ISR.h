#ifndef ISR_H
#define ISR_H

#include "../headers/kernel.h"
#include "../headers/utility.h"

typedef struct {
  uint32 ds;
  uint32 edi;
  uint32 esi;
  uint32 ebp;
  uint32 kern_esp;
  uint32 ebx;
  uint32 edx;
  uint32 ecx;
  uint32 eax;
  uint32 interrupt_number;
  uint32 error;
  uint32 eip;
  uint32 cs;
  uint32 eflag;
  uint32 esp;
  uint32 ss;

} __attribute((packed)) Registers;


void __attribute__((cdecl)) ISRHANDLER(Registers* reg){
  kernelprint("interupt number %d",0,1,reg->interrupt_number);
}


#endif
