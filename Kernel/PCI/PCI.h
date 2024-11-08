#ifndef PCI_H
#define PCI_H

#include "../headers/ports.h"
#include "../headers/utility.h"
#include "Device.h"

#define PCIDATAPORT 0xCFC 
#define PCICMDPORT 0xCF8

uint32 readpci(uint16 bus, uint16 device, uint16 function, uint32 offset);

void writepci(uint16 bus, uint16 device, uint16 function,uint32 offset,uint32 data);

void enumeratedevices();

#endif
