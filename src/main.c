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


	byte glider[3][3] = {
		{0, 0, 1},
		{1, 0, 1},
		{0, 1, 1}
	};
	byte blinker[3][3] = {
		{0, 1, 0},
		{0, 1, 0},
		{0, 1, 0}
	};


	addPattern(w, h, state[0], 3, 3, glider, 0, 0);
	addPattern(w, h, state[0], 3, 3, glider, 10, 10);
	addPattern(w, h, state[0], 3, 3, blinker, 10, 0);
	addPattern(w, h, state[0], 3, 3, blinker, 20, 20);



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