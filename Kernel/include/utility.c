#include "../headers/utility.h"

int kernelstrlen(char *input){
    int count = 0;
    char curchar = input[count];
    while(curchar != 0){
        count++;
        curchar = input[count];
    }
    return count;
}

void kernelprint(char *output){

    int len = kernelstrlen(output); // length of inputed string
    short* vga = (short*)0xb8000; // adress for VgA buffer
    const short color = 0x0F00; // color, white on black

    for (int i = 0; i<len;i++){
      vga[i+80] = color | output[i];
    }
}