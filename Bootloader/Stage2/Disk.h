#ifndef BIOSFUNCTIONS_H
#define BIOSFUNCTIONS_H

#include "../../Kernel/headers/kernel.h"

void __attribute__((cdecl)) ReadDisk(uint8 drive, uint16 cylinder, uint16 sector, uint16 head, uint8 sectorcount, void* buffer);



#endif
