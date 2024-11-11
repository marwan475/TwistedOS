#include "ethernet.h"
#include "networkfunctions.h"
#include "../headers/utility.h"

void handleARPrecv(void* data, int datalen){
    struct ARPpacket *packet = (struct ARPpacket*) data;

    uint8 HWaddr[6];
    uint8 Paddr[4];

    // saving the addr of the sender so we can reply back 
    kernelmemcopy(HWaddr,packet->senderHWaddr,6);
    kernelmemcopy(Paddr,packet->senderPaddr,4);

    uint16 op = ntohs(packet->operation);

    // reply to arp request
    if(op == ARPREQ){
	
    }
}

void ethernethandlerecv(uint8* packet, int len){
 
    struct ethernetframe *frame = (struct ethernetframe*) packet; 

    // skiping ethernet header to get data
    void* data = (void*) packet + sizeof(struct ethernetframe);
    int datalen = len - sizeof(struct ethernetframe);

    uint16 type = ntohs(frame->type);

    if (type == ARP){
	handleARPrecv(data,datalen);
	kernelprint("ARP Packet recived%n");
    }

    if (type == IPV4){
	kernelprint("IPV4 Packet recived%n");
    }
}
