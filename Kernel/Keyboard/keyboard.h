#ifndef KEYBOARD_H
#define KEYBOARD_H

#define dataport 0x60
#define commandport 0x64

#include "../headers/ports.h"

void keyboardDriver();
void keyboardHandler();

#endif