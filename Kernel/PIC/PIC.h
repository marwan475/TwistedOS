#ifndef PIC_H
#define PIC_H

#include "../headers/ports.h"
// Define the I/O ports for the Master and Slave PICs
#define picMinput  0x20  // Master PIC command port
#define picMoutput 0x21  // Master PIC data port
#define picSinput  0xA0  // Slave PIC command port
#define picSoutput 0xA1  // Slave PIC data port

// Remap the PICs to use IRQs starting from 0x20 (32)
void PICremap() {
    // Start initialization sequence
    write8bitportSlow(picMinput, 0x11);  // Master ICW1
    write8bitportSlow(picSinput, 0x11);  // Slave ICW1

    // Set vector offsets
    write8bitportSlow(picMoutput, 0x20);  // Master vector offset
    write8bitportSlow(picSoutput, 0x28);  // Slave vector offset

    // Configure the Master and Slave PICs
    write8bitportSlow(picMoutput, 0x04);  // Master has Slave on IRQ2
    write8bitportSlow(picSoutput, 0x02);  // Slave is connected to Master

    // Set 8086/88 mode
    write8bitportSlow(picMoutput, 0x01);  // Master mode
    write8bitportSlow(picSoutput, 0x01);  // Slave mode

    // Unmask all IRQs
    write8bitportSlow(picMoutput, 0x00);  // Unmask Master IRQs
    write8bitportSlow(picSoutput, 0x00);  // Unmask Slave IRQs
}


#endif

