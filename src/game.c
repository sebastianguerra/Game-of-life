#include <string.h>

#include "game.h"



void setCell(int w, int h, byte matrix[w][h], int i, int j, byte value) {
	if (i >= w || i < 0) return;
	if (j >= h || j < 0) return;

	matrix[i][j] = value;
}

void addValueToCell(int w, int h, byte matrix[w][h], int i, int j, int value) {
	if (i >= w || i < 0) return;
	if (j >= h || j < 0) return;

	matrix[i][j] += value;
}

void changeNeighbours(int w, int h, byte matrix[w][h], int x, int y, int value) {
	addValueToCell(w, h, matrix, x-1, y-1, value);
	addValueToCell(w, h, matrix, x-1, y  , value);
	addValueToCell(w, h, matrix, x-1, y+1, value);

	addValueToCell(w, h, matrix, x  , y-1, value);
	addValueToCell(w, h, matrix, x  , y+1, value);

	addValueToCell(w, h, matrix, x+1, y-1, value);
	addValueToCell(w, h, matrix, x+1, y  , value);
	addValueToCell(w, h, matrix, x+1, y+1, value);
}


void compute (int w, int h, byte from[w][h], byte to[w][h]) {
	memset(to, 0, w * h * sizeof(byte)); 

	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			if (from[i][j] == 3 || from[i][j] >> 1 == 9) {
				// is alive
				addValueToCell(w, h, to, i, j, 1<<4);

				// add neighbours counter
				changeNeighbours(w, h, to, i, j, 1);
			}
		}
	}
}


/*
w1, h1 -> size of matrix

w2, h2 -> size of pattern

x, y -> location in matrix where pattern will be inserted
*/

void addPattern(int w1, int h1, byte matrix[w1][h1], int w2, int h2, byte pattern[w2][h2], int x, int y) {
	for(int i = 0; i < w2; i++ ) {
		for(int j = 0; j < h2; j++ ) {
			if ( i + x >= w1 || j + y >= h1 ) continue;

			byte prev = matrix[i+x][j+y];
			byte new = pattern[i][j];

			if ( prev >> 4 == new) continue; // if the cells are both alive or dead then do nothing

			if (new) { // if the new cell is alive then:
				setCell(w1, h1, matrix, x+i, y+j, prev | 1 << 4);

				// add neighbours counter
				changeNeighbours(w1, h1, matrix, x+i, y+j, 1);
			} else { // if now is dead then:
				setCell(w1, h1, matrix, x+i, y+j, prev & 15);

				// substract from neighbours counter
				changeNeighbours(w1, h1, matrix, x+i, y+j, -1);
			}
		}
	}
}
