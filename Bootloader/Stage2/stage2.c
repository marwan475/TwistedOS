#include "../../Kernel/headers/utility.h"
#include "Disk.h"

void stage2(int LBA){
  kernelprint("hello from c %d%n",LBA);

  void* buffer = (void*)0xBFFF;

  int res = 1;
  while(res && LBA){
    res = ReadFromDisk((uint8)0,LBA,(uint8)1,buffer);

    kernelprint("Disk Read Worked %d%n",res);

    LBA = FindNextCluster(LBA);

    kernelprint("Cluster Found %d%n",LBA);

    console();
  }

  kernelprint("DONE");
}
