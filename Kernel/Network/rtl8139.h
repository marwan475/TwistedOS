#ifndef RTL8139
#define RTL8139

#include "../headers/kernel.h"
#include "../headers/ports.h"
#include "../PCI/PCI.h"

#define RCMD 0x37
#define CONFIG1 0x52
#define BASEADDR 0xC000
#define PCICMD 0x04

void initNIC(){

    // Reading PCI cmd register
    uint32 t = readpci(0,3,0,PCICMD);

    //100 setting bit 2 on for PCI bus mastering
    t |= (1 << 2);

    // writing the command back to the register
    writepci(0,3,0,PCICMD,t);

    readpci(0,3,0,PCICMD);
    if (t == 0x107)
    kernelprint("%n%d%n",t);


    uint16 command_port = BASEADDR + RCMD;
    uint16 data_port = BASEADDR;
    
    // MAC ADDR
    uint8 status= read8bitport(data_port);

    kernelprint("%n%d%n",status);

    status = read8bitport(data_port+0x01);

    kernelprint("%n%d%n",status );

    status = read8bitport(data_port +0x02);

    kernelprint("%n%d%n",status);

    status = read8bitport(data_port +0x03);

    kernelprint("%n%d%n",status);

    status = read8bitport(data_port+ 0x4);

    kernelprint("%n%d%n",status);

    status = read8bitport(data_port +0x5);

    kernelprint("%n%d%n",status);
}

#endif
