#include "../headers/utility.h"
#include "../headers/kernel.h"

int cursor_col = 0;
int cursor_row = 0;
short* vga = (short*)0xb8000; // adress for VGA buffer
const short color = 0x0F00; // color, white on black

int stringcompare(char* s1,char* s2,int size){
    int check = 1;
    for (int i = 0;i < size;i++){
        if (s1[i] != s2[i]){
            check = 0;
            break;
        }
    }
    return check;
}

void clearscreen(){
    for (int i = 0;i<25*80;i++){
      vga[i] = color | ' ';
    }
    cursor_col = 0;
    cursor_row = 0;
}

void console(){
    
    if (cursor_col == 80 && cursor_row == 24){
      cursor_row = 0;
      cursor_col =0;
      clearscreen();
    }

    if (cursor_col < 0) cursor_col = 0;

    if (cursor_col >= 80){
      cursor_col = 0;
      cursor_row++;
    }

    if (cursor_row >= 25){
      cursor_row = 0;
      cursor_col = 0;
      clearscreen();
    }

    if (cursor_col == 0){
        kernelprint("twisted@MSI: ");
    }
    
}

// Returns Length of inputed string
int kernelstrlen(char *input){
    int count = 0;
    char curchar = input[count];
    while(curchar != 0){
        count++;
        curchar = input[count];
    }
    return count;
}

void printhex(uint8 h){
    char* t = "00";
    char* hex = "0123456789ABCDEF";
    t[0] = hex[(h >> 4) & 0xF];
    t[1] = hex[h & 0xF];
    kernelprint(t);
}

char* int_to_string(int value) {
    static char buffer[12];  // Buffer to hold the resulting string (enough for a 32-bit int + sign + null terminator)
    int is_negative = 0;
    int i = 0;

    value = value + 1100000000;

    // Handle 0 explicitly
    if (value == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return buffer;
    }

    // Handle negative numbers
    if (value < 0) {
        is_negative = 1;
        value = -value;
    }

    // Process each digit and store it in the buffer in reverse order
    while (value != 0) {
        buffer[i++] = (value % 10) + '0';
        value /= 10;
    }

    // Add '-' for negative numbers
    if (is_negative) {
        buffer[i++] = '-';
    }

    // Null-terminate the string
    buffer[i] = '\0';

    // Reverse the string to get the correct order
    for (int j = 0; j < i / 2; j++) {
        char temp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = temp;
    }

    return buffer;
}

// prints 32 bit integer
void kernelprintint(int intput,int col, int offset){

    char *output = int_to_string(intput);

    int len = kernelstrlen(output); // length of inputed string
    

    for (int i = 0;i<len;i++){
        vga[col + i + 80*offset ] = color | output[i]; // add char to vga buffer at location
    }
}

// Writes inputed string to VGA buffer, at col (column) , offset (row) FORMAT: %n is new line %d is int
void kernelprint(char *output, ...){

    int col = cursor_col;
    int offset = cursor_row;
    // VGA buffer is 80x25, 80 columns 25 rows

    int* args = (int*)&output+1; // creating pointer to varargs

    int argc = -1;
    
    int state = STATE_NORMAL;

    int len = kernelstrlen(output); // length of inputed string
    
    int count = 0;

    for (int i = 0; i<len;i++){
      

      // Checking for formating special char
      switch (output[i]) {
        case '%':
            {state = STATE_FORMAT; i++;}
            break;
      }

      if (state == STATE_FORMAT){
        switch (output[i]) {
          case 'd':
            {argc++;kernelprintint(args[argc],col+count,offset);state = STATE_NORMAL;} // moving to next var arg
            break;
          case 'h':
            {argc++;printhex(args[argc]);state = STATE_NORMAL;} // moving to next var arg
            break;
          case 'n':
            {offset++;cursor_row++;count = -1;cursor_col = -1;state = STATE_NORMAL;col = 0;}
            break;
        }
      }else {vga[col + count + 80*offset ] = color | output[i];} // add char to vga buffer at location

      count++;
      cursor_col++;
      

    }
}
