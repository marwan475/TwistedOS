#ifndef PCNETDRIVER_H
#define PCNETDRIVER_H

#include "../PCI/Device.h"
#include "../PCI/PCI.h"

#define MACADDR0 0x0
#define MACADDR2 0x02
#define MACADDR4 0x04
#define NETDATAPORT 0x10
#define NETADDRPORT 0x12
#define RESETPORT 0x14
#define BUSDATAPORT 0x16

// buffer discriptor
struct Bdiscriptor {
	uint32 addr;
	uint32 flags;
	uint32 flags2;
	uint32 ava;
}__attribute__((packed));

struct initblock {
	uint16 mode;
	unsigned reserved1 : 4;
	unsigned numsend : 4;
	unsigned reserved2 : 4;
	unsigned numrec : 4;
	uint64 physaddr : 48;
	uint16 reserved3;
	uint64 logicaladdr;
	uint32 recbuffDaddr;
	uint32 sendbuffDaddr;
}__attribute__((packed));

struct Bdiscriptor* senddiscriptor;
struct Bdiscriptor* recdiscriptor;

uint8 cursendbuffer = 0;
uint8 currecbuffer = 0;

uint8 sendbuffer[2*1024+15][8];
uint8 recbuffer[2*1024+15][8];
uint8 sendDescriptionbuffer[2*1024+15];
uint8 recDescriptionbuffer[2*1024+15];

struct initblock iblock;

struct Deviceinfo NIC;

int reset(){
	read16bitport(NIC.portbase + RESETPORT);
	write16bitport(NIC.portbase + RESETPORT,0x0);
	return 10;
}

int initNIC(){
	NIC = getdevice(0,3,0);
	uint8 MAC0 = read16bitport(NIC.portbase + MACADDR0)%256;
	uint8 MAC1 = read16bitport(NIC.portbase + MACADDR0)/256;

	uint8 MAC2 = read16bitport(NIC.portbase + MACADDR2)%256;
	uint8 MAC3 = read16bitport(NIC.portbase + MACADDR2)/256;

	uint8 MAC4 = read16bitport(NIC.portbase + MACADDR4)%256;
	uint8 MAC5 = read16bitport(NIC.portbase + MACADDR4)/256;

	kernelprint(" %d %n%d %n%d %n%d %n%d %n%d ", MAC0,MAC1,MAC2,MAC3,MAC4,MAC5);
}



#endif
