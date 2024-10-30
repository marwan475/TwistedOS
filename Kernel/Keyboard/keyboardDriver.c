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

// returns char based on scan code
char scan_code_to_char(uint8 code) {
    switch (code) {
        case 0x1E: return 'A';
        case 0x30: return 'B';
        case 0x2E: return 'C';
        case 0x20: return 'D';
        case 0x12: return 'E';
        case 0x21: return 'F';
        case 0x22: return 'G';
        case 0x23: return 'H';
        case 0x17: return 'I';
        case 0x24: return 'J';
        case 0x25: return 'K';
        case 0x26: return 'L';
        case 0x32: return 'M';
        case 0x31: return 'N';
        case 0x18: return 'O';
        case 0x19: return 'P';
        case 0x10: return 'Q';
        case 0x13: return 'R';
        case 0x1F: return 'S';
        case 0x14: return 'T';
        case 0x16: return 'U';
        case 0x2F: return 'V';
        case 0x11: return 'W';
        case 0x2D: return 'X';
        case 0x15: return 'Y';
        case 0x2C: return 'Z'; 
        case 0x35: return '/';
        case 0x34: return '.';
        case 0x39: return ' '; // space
        default: return '\0';       // Return null character for unknown codes
    }
}

void keyboardHandler() {
    uint8 key = read8bitport(dataport); // Read the scan code from the keyboard

    char keychar[2];
    keychar[1] = '\0';

    // if key is not a break code
    if (key < 0x80){

        char k = scan_code_to_char(key);

        keychar[0] = k;
        kernelprint(keychar);

        /*
        // Create a buffer for the hexadecimal representation
        char keychar[5]; // "0x00" + null terminator
        const char *hex = "0123456789ABCDEF";

        // Format the scan code as a hexadecimal string
        keychar[0] = '0';
        keychar[1] = 'x';
        keychar[2] = hex[(key >> 4) & 0x0F]; 
        keychar[3] = hex[key & 0x0F];        
        keychar[4] = '\0';                    

        kernelprint(keychar, 0, 24); // Print the hex representation
        */
    }

}

/*
A 0x1E
B 0x30
C 0x2E
D 0x20
E 0x12
F 0x21
G 0x22
H 0x23
I 0x17
J 0x24
K 0x25
L 0x26
M 0x32
N 0x31
O 0x18
P 0x19
Q 0x10
R 0x13
S 0x1F
T 0x14
U 0x16
V 0x2F
W 0x11
X 0x2D
Y 0x15
Z 0x2C

ENTER 0x1C
SHIFT 0x2A
CTRL 0x1D
TAB 0x0F
/ 0x35
. 0x34
SPACE 0x39
 */

