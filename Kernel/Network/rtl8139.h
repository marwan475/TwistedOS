#ifndef RTL8139
#define RTL8139

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

#define RECBUFFERSIZE 8192
#define RPMASK (~3)

uint32* recvbuffer;
uint32* transmitbuffer;

void initNIC(){

    // Reading PCI cmd register
    uint32 t = readpci(0,3,0,PCICMD);

    //100 setting bit 2 on for PCI bus mastering
    t |= (1 << 2);

    // writing the command back to the register
    writepci(0,3,0,PCICMD,t);

    readpci(0,3,0,PCICMD);
    if (t == 0x107)
    kernelprint("PCI bus mastering enabled for NIC%n");

    // insuring device is in active high, default value of LWACT(bit 4 in) and LWPTN is 0 meaning the LWAKE pin is in active high
    write8bitportSlow(BASEADDR + CONFIG1,0x00);
    
    // Software reset
    // Clear RX and TX buffers
    // seting bit 4 in CMD register to 0 (RST) force software rest once done the bit is set back to 0
    // bug in qemu, bit 4 is set to 1, i printed before and after doing reset to confirm reset is working and it is

    write8bitport(BASEADDR + RCMD, 0x10);
    // wait until software rest is complete, check for RST bit
    while((read8bitport(BASEADDR+RCMD) & 0x10) != 0){}

    // init recive buffer, need to send the card a memroy location it can use to store recv data
    
    // allocate space for reciving needs to be 8192+16 bytes
    recvbuffer = (uint32*)kernelmalloc(8192+16+1500);

    write32bitport(BASEADDR+RBUFFER,(uint32)recvbuffer);

    transmitbuffer = (uint32*)kernelmalloc(1536);


    // setting interrupt mask, at reset all interrupts are disabled so all bits are sit to 0
    
    // set all bits to high except reserved
    write16bitport(BASEADDR+INTMASK,0x0005);

    
    
    // writing to RCR to tell it what packets to accept
    // setting warp bit to high to tell it to overflow if packet is big
    write32bitport(BASEADDR+RCR,0x1F | (1 << 7));
   
    // enabling reciving and transmiting by setting comand bits 2 and 3 to high
    write8bitport(BASEADDR+RCMD,0x0C);
    
}

uint32 curpacket = 0;

void recvpacket(){
    uint16 * buff = (uint16*)(recvbuffer + curpacket);

    // get packet length from packet header
    uint16 packetlength = *(buff + 1);

    // skip header
    buff += 2;

    // Free this
    uint8* packet = kernelmalloc(packetlength);

    curpacket = (curpacket + packetlength + 4 + 3) & RPMASK;

    if (curpacket > RECBUFFERSIZE) curpacket -= RECBUFFERSIZE;

    write16bitport(BASEADDR+ CAPR,curpacket - 0x10);

    kernelprint("Packet Recieved%n");

}

void NICISR43handler(){

    while (1){
	uint16 status = read16bitport(BASEADDR+INTSTATUS);

	if (status == 0) break;

	write16bitport(BASEADDR+INTSTATUS,status);

	//bit 0 is Recive OK
	if (status == 0x0001){
	    recvpacket(); 
	}

	// bit 3 is transmit ok
	if (status == 0x0004){

	}

    }
    
    
    

    


}



#endif
