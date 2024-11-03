#ifndef DISK_H
#define DISK_H

#include "../../Kernel/headers/kernel.h"

int __attribute__((cdecl)) ReadDisk(uint8 drive, uint16 cylinder, uint16 sector, uint16 head, uint8 sectorcount, void* buffer);

void LBAtoCHS(int LBA, uint16* cylinder, uint16* sector, uint16* head){
	*sector = (LBA % 18) + 1;
	*cylinder = (LBA/18) / 2;
	*head = (LBA / 18) % 2;
}

int ReadFromDisk(uint8 disknumber, int LBA, uint8 sectors, void* buffer){
	uint16 cylinder = 0;
	uint16 sector = 0;
	uint16 head = 0;

	LBAtoCHS(LBA,&cylinder,&sector,&head);

	return ReadDisk(disknumber,cylinder,sector,head,sectors,buffer);
}

int __attribute__((cdecl)) FindNextCluster(int LBA);

#endif
