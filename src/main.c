#include <string.h>
#include <unistd.h>
#include <ncurses.h>


#include "game.h"



void show (int w, int h, byte matrix[w][h], int rows, int columns, int x, int y);



int main() {
	initscr();

	curs_set(0);
	noecho();
	cbreak();
	timeout(0);

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
	byte toad[2][4] = {
		{0, 1, 1, 1},
		{1, 1, 1, 0},
	};
	byte gosperglidergun[9][50] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
		{0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
		{1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,1,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	};


	addPattern(w, h, state[0], 3, 3,  glider,  0, 20);
	addPattern(w, h, state[0], 3, 3,  glider, 10, 20);
	addPattern(w, h, state[0], 3, 3, blinker, 10, 20);
	addPattern(w, h, state[0], 3, 3, blinker, 20, 20);
	addPattern(w, h, state[0], 2, 4,    toad, 80, 20);
	addPattern(w, h, state[0], 3, 3,  glider,  0, 40);
	addPattern(w, h, state[0], 9, 50, gosperglidergun, 1, 1);

	int x = 0, y = 0;

	for (int i = 0; ; i++) {
		int c = getch();
		switch(c){
			case 'h':
				x++;break;
			case 'j':
				y--;break;
			case 'k':
				y++;break;
			case 'l':
				x--;break;
		}
		flushinp();
		getmaxyx(stdscr, row, col);
		show(w, h, state[i%2], row, col, y, x);
		compute(w, h, state[i%2], state[(i+1)%2]);
		napms(10);
	}

	endwin();
	return 0;
}



void show (int w, int h, byte matrix[w][h], int rows, int columns, int x, int y) {
	clear();
	for(int i = 0; i < w; i++) {
		for(int j = 0; j < h; j++) {
			char r;
			if (matrix[i][j] & 1<<4)
				r = 'O';
			else 
				r = '.';
			mvaddch(i+x, j+y, r);
		}
	}
	refresh();
}