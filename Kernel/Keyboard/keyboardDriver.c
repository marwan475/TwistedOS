#include "keyboard.h"
#include "../headers/utility.h"

void keyboardDriver(){

    // clear data in the keyboard buffer
    while(read8bitport(commandport) == 0x1)
        read8bitport(dataport);

    // enabling keyboard
    write8bitportSlow(commandport,0xAE);
    write8bitportSlow(commandport,0x20);

    // reading status
    uint8 status = (read8bitport(dataport) | 1) & ~0x10;

    // setup new config
    write8bitportSlow(commandport,0x60);
    write8bitportSlow(dataport,status);

    // enable keyboard scanning
    write8bitportSlow(dataport,0xF4);

}

void keyboardHandler() {
    uint8 key = read8bitport(dataport); // Read the scan code from the keyboard

    // Create a buffer for the hexadecimal representation
    char keychar[5]; // "0x00" + null terminator
    const char *hex = "0123456789ABCDEF";

    // Format the scan code as a hexadecimal string
    keychar[0] = '0';
    keychar[1] = 'x';
    keychar[2] = hex[(key >> 4) & 0x0F]; // Upper nibble
    keychar[3] = hex[key & 0x0F];        // Lower nibble
    keychar[4] = '\0';                    // Null-terminate the string

    kernelprint(keychar, 0, 24); // Print the hex representation
}