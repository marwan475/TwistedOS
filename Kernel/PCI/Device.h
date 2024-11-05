#ifndef DEVICE_H
#define DEVICE_H

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

//Base address register
struct Bar {
	int pre;
	uint8* addr;
	uint32 size;
	int type; // 1 for I/O 0 for mem map
};

#endif
