#include <string.h>

#include "game.h"



void setCell(int w, int h, byte matrix[w][h], int i, int j, byte value) {
	if (i >= w || i < 0) return;
	if (j >= h || j < 0) return;

	matrix[i][j] = value;
}

void addValueToCell(int w, int h, byte matrix[w][h], int i, int j, byte value) {
	if (i >= w || i < 0) return;
	if (j >= h || j < 0) return;

	matrix[i][j] += value;
}

void compute (int w, int h, byte from[w][h], byte to[w][h]) {
	memset(to, 0, w * h * sizeof(byte)); 

	for (int i = 0; i < w; i++) {
		for (int j = 0; j < h; j++) {
			if (from[i][j] == 3 || from[i][j] >> 1 == 9) {
				// is alive
				addValueToCell(w, h, to, i, j, 1<<4);

				// add neighbours counter
				addValueToCell(w, h, to, i-1, j-1, 1);
				addValueToCell(w, h, to, i-1, j  , 1);
				addValueToCell(w, h, to, i-1, j+1, 1);

				addValueToCell(w, h, to, i  , j-1, 1);
				addValueToCell(w, h, to, i  , j+1, 1);

				addValueToCell(w, h, to, i+1, j-1, 1);
				addValueToCell(w, h, to, i+1, j  , 1);
				addValueToCell(w, h, to, i+1, j+1, 1);
			}
		}
	}
}