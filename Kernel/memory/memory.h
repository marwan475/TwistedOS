#ifndef MEMORY_H
#define MEMORY_H

#include "../headers/kernel.h"
#include "../headers/utility.h"

#define HEAP_MEMORY 0x100000
#define BLOCK_SIZE 1
#define BLOCKS 40000

void kernelmemset(uint8* buf,int count,uint8 set);
void heapinit();
uint8* kernelmalloc(int bytes);
void kernelfree(uint8* mem, int bytes);

#endif
