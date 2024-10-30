#include "memory.h"

//contains info on what blocks are allocated
//each block will e 1024 bytes
//40 blocks total
//will need to init each value to 0
uint8 bitmap[BLOCKS];
uint8 *heap = (uint8*) HEAP_MEMORY;


void kernelmemset(uint8* buf,int count,uint8 set){
  for (int i = 0; i < count;i++){
    buf[i] = set;
  } 
}

void heapinit(){
  kernelmemset(bitmap,BLOCKS,0);
}

// calculates how many blocks of memory is need
int calculateblocks(int bytes){
  int total = bytes;
  int count = 0; 

  while(1){
    total = total - BLOCK_SIZE;
    if (total <= 0){
      count++;
      break;
    }
    count++;
  }

  return count;
}

// returns index of bit map where requested memory first fits
int findFirstFit(int blocks){
  int count = 0;

  for(int i =0;i < BLOCKS;i++){
    if ( bitmap[i] == 0){
      count++;
    }else{
      count = 0;
    }

    if (count == blocks) return i - count + 1;
  }

  return -1;
}

// sets bit map at index to free or taken
void setbitmap(int index, int blocks, int value){
  for (int i = 0;i < blocks;i++){
    bitmap[index+i] = value;
  }
}

// returns offset from heap memory pointer
int findoffset(int index){
  return index * BLOCK_SIZE;
}

// allocates heap memory based on bytes needed
uint8* kernelmalloc(int bytes){
  int blocks = calculateblocks(bytes);
  int index = findFirstFit(blocks);

  if (index == -1){kernelprint("HEAP OVERFLOW");panic();}

  setbitmap(index,blocks,1);

  int offset = findoffset(index);

  uint8* alloc = (uint8*)((uint32)heap + (uint32) offset);

  kernelprint("Memory Allocated %d %n",index);
  return alloc;

  
}

// return bitmap index given memory address
int findindex(uint8* mem){
  uint8* offset = (uint8*)((uint32)mem - (uint32)heap);
  return (int) offset/BLOCK_SIZE;
}

// frees alocated memory from bit map
void kernelfree(uint8* mem, int bytes){
  int index = findindex(mem);
  int blocks = calculateblocks(bytes);

  setbitmap(index,blocks,0);

  kernelprint("Memory freed %d %n",index);
}

