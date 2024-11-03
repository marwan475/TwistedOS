#include "physicalmemory.h"

int memsize = ENDMEM - STARTMEM;
int maxblocks = (ENDMEM - STARTMEM)/BLOCKSIZE;

//contains info on what blocks are allocated
//will need to init each value to 0
uint8 bitmap[(ENDMEM - STARTMEM)/BLOCKSIZE];
uint8 *heap = (uint8*) STARTMEM;


void kernelmemset(uint8* buf,int count,uint8 set){
  for (int i = 0; i < count;i++){
    buf[i] = set;
  } 
}

void physicalmemoryinit(){
  kernelmemset(bitmap,maxblocks,0);
  // set kernel space as being used
  kernelmemset(bitmap,KERNELSIZE/BLOCKSIZE,1);
}

// calculates how many blocks of memory is need
int calculateblocks(int bytes){
  int total = bytes;
  int count = 0; 

  while(1){
    total = total - BLOCKSIZE;
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

  for(int i =0;i < maxblocks;i++){
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
  return index * BLOCKSIZE;
}

// allocates heap memory based on bytes needed
uint8* kernelmalloc(int bytes){
  int blocks = calculateblocks(bytes);
  int index = findFirstFit(blocks);

  if (index == -1){kernelprint("HEAP OVERFLOW");panic();}

  setbitmap(index,blocks,1);

  int offset = findoffset(index);

  uint8* alloc = (uint8*)((uint32)heap + (uint32) offset);

  kernelprint("Memory Allocated %d %n %d %n",index,blocks);
  return alloc;

  
}

// return bitmap index given memory address
int findindex(uint8* mem){
  uint8* offset = (uint8*)((uint32)mem - (uint32)heap);
  return (int) offset/BLOCKSIZE;
}

// frees alocated memory from bit map
void kernelfree(uint8* mem, int bytes){
  int index = findindex(mem);
  int blocks = calculateblocks(bytes);

  setbitmap(index,blocks,0);

  kernelprint("Memory freed %d %n %d %n",index, blocks);
}

