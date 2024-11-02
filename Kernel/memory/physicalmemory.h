#ifndef PHYSICALMEMORY_H
#define PHYSICALMEMORY_H

#include "../headers/kernel.h"
#include "../headers/utility.h"

#define HEAP_MEMORY 0x25F900 // 1.44mb higher then kernel load adress
#define BLOCK_SIZE 4096 // bytes per block
#define BLOCKS 1024 // total amount of blocks availble for allocation

void kernelmemset(uint8* buf,int count,uint8 set);
void heapinit();
uint8* kernelmalloc(int bytes);
void kernelfree(uint8* mem, int bytes);

#endif
