#ifndef RTL8139_H
#define RTL8139_H

#include "../headers/kernel.h"
#include "../headers/ports.h"
#include "../PCI/PCI.h"
#include "../memory/physicalmemory.h"

#define RCMD 0x37 // cmd regist offset
#define CONFIG1 0x52 // config 1 register offset
#define BASEADDR 0xC000 // base io addr
#define PCICMD 0x04 // pci cmd offset
#define RBUFFER 0x30 // recv buffer addr offset
#define INTMASK 0x3C // interrupt mask register
#define INTSTATUS 0x3E // interrupt status register
#define RCR 0x44 // Recive Config Register
#define CAPR 0x38 // current address of packet read
#define MACADDR1 0x00
#define MACADDR2 0x04

#define RECBUFFERSIZE 8192
#define RPMASK (~3)

void initNIC();
void NICISR43handler();

#endif
