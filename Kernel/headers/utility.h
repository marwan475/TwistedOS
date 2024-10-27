#ifndef UTILITY_H
#define UTILITY_H

#define STATE_NORMAL 0
#define STATE_FORMAT 1

int kernelstrlen(char *input);

void kernelprint(char *output, int col ,int offset, ...);

#endif