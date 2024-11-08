#include "rtl8139.h"

uint32* recvbuffer;
uint32* transmitbuffer;

uint32 curpacket = 0;

// TXAD registers, used for sending data, need to use a diffrent one each time
uint8 TXAD[4] = {0x20,0x24,0x28,0x2C};
uint8 TSD[4] = {0x10,0x14,0x18,0x1C};
int curtx = 0;

uint8 MAC[6];

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

void readMAC(){ 

    uint32 MAC1 = read32bitport(BASEADDR + MACADDR1);
    uint16 MAC2 = read16bitport(BASEADDR + MACADDR2);

    MAC[0] = MAC1 >> 0;
    MAC[1] = MAC1 >> 8;
    MAC[2] = MAC1 >> 16;
    MAC[3] = MAC1 >> 24;
    MAC[4] = MAC2 >> 0;
    MAC[5] = MAC2 >> 8;

    kernelprint("MAC: %h:%h:%h:%h:%h:%h %n",MAC[0],MAC[1],MAC[2],MAC[3],MAC[4],MAC[5]);
    
}

void recvpacket(){
    uint16 * buff = (uint16*)(recvbuffer + curpacket);

    // first 16 bits is rtl6139 recieve packet header
    // second 16 bits is packet length
    // get packet length from packet header
    uint16 packetlength = *(buff + 1); // skip first 16 bits and get packet length

    // skip header 32bits and point to data
    buff += 2;

    // Free this
    // copy packet data
    uint8* packet = kernelmalloc(packetlength);
    kernelmemcopy(packet,(uint8*)buff,packetlength);

    // +4 to packet length includes header the align pointer using 3 and RPMASK
    curpacket = (curpacket + packetlength + 4 + 3) & RPMASK;

    if (curpacket > RECBUFFERSIZE) curpacket -= RECBUFFERSIZE;

    write16bitport(BASEADDR+ CAPR,curpacket - 0x10);

    kernelprint("Packet Recieved%n");

    kernelfree(packet,packetlength);

}

void sendpacket(void* frame, uint32 len){
    
    write32bitport(BASEADDR + TXAD[curtx],(uint32)frame);
    write32bitport(BASEADDR + TSD[curtx],len);

    curtx++;

    if(curtx > 3) curtx = 0;
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


