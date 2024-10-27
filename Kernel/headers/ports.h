#ifndef PORTS_H
#define PORTS_H

#include "kernel.h"

void write8bitport(uint16 portnumber,uint8 data);
uint8 read8bitport(uint16 portnumber);
void write8bitportSlow(uint16 portnumber,uint8 data);

void write16bitport(uint16 portnumber,uint16 data);
uint8 read16bitport(uint16 portnumber);
void write16bitportSlow(uint16 portnumber,uint16 data);

void write32bitport(uint16 portnumber,uint32 data);
uint8 read32bitport(uint16 portnumber);
void write32bitportSlow(uint16 portnumber,uint32 data);

#endif 