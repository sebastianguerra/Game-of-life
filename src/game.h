#ifndef GAME_H
#define GAME_H



typedef unsigned char byte;





void setCell(int w, int h, byte matrix[w][h], int i, int j, byte value);

void addValueToCell(int w, int h, byte matrix[w][h], int i, int j, int value);

void compute (int w, int h, byte from[w][h], byte to[w][h]);

void addPattern(int w1, int h1, byte matrix[w1][h1], int w2, int h2, byte pattern[w2][h2], int x, int y);


#endif