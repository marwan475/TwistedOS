#ifndef PHYSICALMEMORY_H
#define PHYSICALMEMORY_H

#include "../headers/kernel.h"
#include "../headers/utility.h"

#define BLOCKSIZE 4096

#define STARTMEM 0x00100000
#define ENDMEM 0x00FFFFFF
#define KERNELSIZE 1440000


void kernelmemset(uint8* buf,int count,uint8 set);
void physicalmemoryinit();
uint8* kernelmalloc(int bytes);
void kernelfree(uint8* mem, int bytes);

#endif
