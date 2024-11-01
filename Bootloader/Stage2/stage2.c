#include "../../Kernel/headers/utility.h"
#include "Disk.h"

void stage2(int LBA){
  kernelprint("hello from c %d%n",LBA);

  void* buffer = (void*)0xBFFF;

  int res = ReadFromDisk((uint8)0,LBA,(uint8)1,buffer);

  kernelprint("Worked %d",res);
}
