#ifndef GAME_H
#define GAME_H



typedef unsigned char byte;





void setCell(int w, int h, byte matrix[w][h], int i, int j, byte value);

void addValueToCell(int w, int h, byte matrix[w][h], int i, int j, byte value);

void compute (int w, int h, byte from[w][h], byte to[w][h]);



#endif