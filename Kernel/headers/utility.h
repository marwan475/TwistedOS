#ifndef UTILITY_H
#define UTILITY_H

#define STATE_NORMAL 0
#define STATE_FORMAT 1

extern int cursor_col;
extern int cursor_row;

int stringcompare(char* s1,char* s2,int size);

int kernelstrlen(char *input);

void kernelprint(char *output, ...);

void console();

void clearscreen();

#endif
