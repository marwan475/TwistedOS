#ifndef PCI_H
#define PCI_H

#include "../headers/ports.h"
#include "../headers/utility.h"

#define PCIDATAPORT 0xCFC 
#define PCICMDPORT 0xCF8

struct Deviceinfo {
	uint32 portbase;
	uint32 interrupt;
	uint16 bus;
	uint16 device;
	uint16 function;
	uint16 vendor;
	uint16 deviceid;
	uint8 classid;
	uint8 subclass;
	uint8 interface;
	uint8 revision;
};

// Read/write data to/from PCI->bus->device->function->offset
uint32 readpci(uint16 bus, uint16 device, uint16 function, uint32 offset) {
    uint32 id = 0x80000000 | (bus << 16) | (device << 11) | (function << 8) | (offset & 0xFC);
    write32bitport(PCICMDPORT, id);
    return read32bitport(PCIDATAPORT);
}


void writepci(uint16 bus, uint16 device, uint16 function,uint32 offset,uint32 data){

	uint32 id = 0x1 << 31 | ((bus & 0xFF) << 16 ) | ((device & 0x1F) << 11) | ((function & 0x07) << 8) | (offset & 0xFC);

	write32bitport(PCICMDPORT,id);
	write32bitport(PCIDATAPORT,data);
}

int devicefunctioninfo(uint16 bus, uint16 device){
	return readpci(bus,device,0,0x0E) & (1<<7);
}

struct Deviceinfo getdeviceinfo(uint16 bus, uint16 device, uint16 function){
	
	struct Deviceinfo res;

	res.bus = bus;
	res.device = device;
	res.function = function;
	
	uint32 vendor_device = readpci(bus, device, function, 0x00);
   	res.vendor = vendor_device & 0xFFFF;
    	res.deviceid = (vendor_device >> 16) & 0xFFFF;

    	res.classid = (uint8) (readpci(bus, device, function, 0x08) >> 24);
    	res.subclass = (uint8) (readpci(bus, device, function, 0x08) >> 16);
    	res.interface = (uint8) (readpci(bus, device, function, 0x08) >> 8);
    	res.revision = (uint8) readpci(bus, device, function, 0x08);
    	res.interrupt = (uint8) readpci(bus, device, function, 0x3C);

	return res;
}

void enumeratedevices(){
	for (int bus = 0;bus<8;bus++){
		for (int device = 0; device < 32; device++){

			int numfunctions = 0;
			// checks if device has functions
			int check = devicefunctioninfo(bus,device);
			if (check) numfunctions = 8;
			else numfunctions = 1;

			for (int function = 0;function < numfunctions; function++){
				struct Deviceinfo d = getdeviceinfo(bus,device,function);

				if (d.vendor == 0x0000 || d.vendor == 0xFFFF) continue;

				kernelprint("%nbus : %h device: %h ", bus & 0xFF, device & 0xFF);
				kernelprint("function : %h ",function & 0xFF);
				kernelprint("Vendor: %h%h ",(d.vendor & 0xFF00) >> 8,d.vendor & 0xFF);
				kernelprint("Device: %h%h %n",(d.deviceid & 0xFF00) >> 8,d.deviceid & 0xFF);
				
				

			}
		}
	}
}

#endif
