#include <string.h>
#include <unistd.h>
#include <ncurses.h>


typedef unsigned char byte;


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

void show (int w, int h, byte matrix[w][h], int rows, int columns) {
	for(int i = 0; i < w && i < rows; i++) {
		for(int j = 0; j < h && j < columns; j++) {
			char r;
			if (matrix[i][j] & 1<<4)
				r = 'O';
			else 
				r = ' ';
			mvaddch(i, j, r);
		}
	}
	refresh();
}

int main() {
	initscr();

	curs_set(0);

	int row, col;
	getmaxyx(stdscr, row, col);


	int w = row*2, h = col*2;

	byte state[2][w][h];
	memset(state, 0, 2 * w * h * sizeof(byte)); 

	/* glider */
	byte istate[5][5] = {
		{ 0,  0,  1,  1,  1},
		{ 1,  1,  3, 17,  2},
		{ 1, 17,  5, 19,  3},
		{ 1,  2, 19, 18,  2},
		{ 0,  1,  2,  2,  1}
	};

	/* blinker */
	// byte istate[5][5] = {
	// 	{ 0,  1,  1,  1,  0},
	// 	{ 0,  2, 17,  2,  0},
	// 	{ 0,  3, 18,  3,  0},
	// 	{ 0,  2, 17,  2,  0},
	// 	{ 0,  1,  1,  1,  0}
	// };

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			state[0][i][j] = istate[i][j];
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