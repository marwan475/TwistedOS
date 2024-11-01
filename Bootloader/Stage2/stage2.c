#include "../../Kernel/headers/utility.h"
#include "Disk.h"


void copy(uint8* dest, uint8* source, int bytes){
  for (int i = 0; i < bytes; i++){
    dest[i] = source[i];
  }
}

typedef void (*KernelStart)();

uint32* kernel = (uint32*)0x100000;

void stage2(int LBA){
  
  void* buffer = (void*)0xCCFD; // 1 sector after full fat table load in

  int next = 0;
  uint32* kernel_buffer = kernel;
  int res = 1;
  while(res){
    res = ReadFromDisk((uint8)0,LBA,(uint8)1,buffer);

    copy((uint8*)kernel_buffer,(uint8*)buffer,512);

    next = FindNextCluster(LBA);

    kernel_buffer = (uint32*)((uint32)kernel_buffer +  (uint32)0x200);

    // load extra sector incase
    if (next == 0){
      res = ReadFromDisk((uint8)0,LBA,(uint8)1,buffer);
      copy((uint8*)kernel_buffer,(uint8*)buffer,512);
      break;
    }

    LBA = next;
  }

  KernelStart kernelStart = (KernelStart)kernel;
  kernelStart();

  kernelprint("FAIL");

}
