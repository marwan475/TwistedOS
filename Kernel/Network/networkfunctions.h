#ifndef NETWORKFUNCTIONS_H
#define NETWORKFUNCTIONS_H

#include "../headers/kernel.h"

// 8-bit
uint8 ntohb(uint8 netbyte) {
    return netbyte;
}

uint8 htonb(uint8 hostbyte) {
    return hostbyte;
}

// 16-bit
uint16 ntohs(uint16 netshort) {
    return (netshort << 8) | (netshort >> 8);
}

uint16 htons(uint16 hostshort) {
    return (hostshort << 8) | (hostshort >> 8);
}

// 32-bit
uint32 ntohl(uint32 netlong) {
    return ((netlong & 0x000000FF) << 24) |
           ((netlong & 0x0000FF00) << 8)  |
           ((netlong & 0x00FF0000) >> 8)  |
           ((netlong & 0xFF000000) >> 24);
}

uint32 htonl(uint32 hostlong) {
    return ((hostlong & 0x000000FF) << 24) |
           ((hostlong & 0x0000FF00) << 8)  |
           ((hostlong & 0x00FF0000) >> 8)  |
           ((hostlong & 0xFF000000) >> 24);
}

#endif
