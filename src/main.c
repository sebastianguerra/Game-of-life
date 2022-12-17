#include <string.h>
#include <unistd.h>
#include <ncurses.h>


#include "game.h"



void show (int w, int h, byte matrix[w][h], int rows, int columns);



int main() {
	initscr();

	curs_set(0);

	int row, col;
	getmaxyx(stdscr, row, col);


	int w = row*2, h = col*2;

	byte state[2][w][h];
	memset(state, 0, 2 * w * h * sizeof(byte)); 

	/* glider */
	byte glider[5][5] = {
		{ 0,  0,  1,  1,  1},
		{ 1,  1,  3, 17,  2},
		{ 1, 17,  5, 19,  3},
		{ 1,  2, 19, 18,  2},
		{ 0,  1,  2,  2,  1}
	};

	/* blinker */
	byte blinker[5][5] = {
		{ 0,  1,  1,  1,  0},
		{ 0,  2, 17,  2,  0},
		{ 0,  3, 18,  3,  0},
		{ 0,  2, 17,  2,  0},
		{ 0,  1,  1,  1,  0}
	};

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			state[0][i][j] = glider[i][j];
		}
	}

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			state[0][i][j+10] = blinker[i][j];
		}
	}



	for (int i = 0; ; i++) {
		getmaxyx(stdscr, row, col);
		show(w, h, state[i%2], row, col);
		compute(w, h, state[i%2], state[(i+1)%2]);
		napms(100);
	}

	endwin();
	return 0;
}



void show (int w, int h, byte matrix[w][h], int rows, int columns) {
	for(int i = 0; i < w && i < rows; i++) {
		for(int j = 0; j < h && j < columns; j++) {
			char r;
			if (matrix[i][j] & 1<<4)
				r = 'O';
			else 
				r = '_';
			mvaddch(i, j, r);
		}
	}
	refresh();
}