#ifndef ETHERNET_H
#define ETHERNET_H

#include "../headers/kernel.h"

#define ARP 0x0806
#define IPV4 0x0800

#define ARPREQ 1
#define ARPRPLY 2

// Ethernet Frame
struct ethernetframe{
	uint8 destinationMAC[6];
	uint8 sourceMAC[6];
	uint16 type; // ARP or IPV4
};

// ARP packet
struct ARPpacket{
	uint16 hardware;
	uint16 protocol;
	uint8 hardwarelen;
	uint8 protocollen;
	uint16 operation;
	uint8 senderHWaddr[6];
	uint8 senderPaddr[4];
	uint8 targetHWaddr[6];
	uint8 targetPaddr[4];
}__attribute__((packed));

void ethernethandlesend();

void ethernethandlerecv(uint8* packet, int len);

#endif
