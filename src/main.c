#include <string.h>
#include <unistd.h>
#include <ncurses.h>


#include "game.h"



void show (int w, int h, byte matrix[w][h], int rows, int columns, int x, int y);





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





int main() {
	///////////////////////////////////////////////////////
	//// Inicializacion ncurses
	///////////////////////////////////////////////////////

	initscr(); // Inicia ncurses.

	curs_set(0); // Elimina el cursor
	noecho();    // Evita que se dibuje la tecla presionada
	cbreak();    // evita el buffering pero CTRL-C sigue funcionando normal
	timeout(0);  // No espera el input en un getch()




	// row, col: Tamano terminal
	int row, col;
	getmaxyx(stdscr, row, col);

	// w, h: Tamano matriz
	int w = row*2, h = col*2;

	// state: Pareja de matrices de tamano WxH que almacenan informacion
	// 		  del estado actual del juego. 
	//		  Se usa una pareja para alternar la matriz en la que se escribira 
	//		  la informacion en cada turno.
	byte state[2][w][h];
	memset(state, 0, 2 * w * h * sizeof(byte)); // inicializa los arrays a 0


	// Patrones de prueba
	addPattern(w, h, state[0], 3, 3,  glider,  0, 20);
	addPattern(w, h, state[0], 3, 3,  glider, 10, 20);
	addPattern(w, h, state[0], 3, 3, blinker, 10, 20);
	addPattern(w, h, state[0], 3, 3, blinker, 20, 20);
	addPattern(w, h, state[0], 2, 4,    toad, 80, 20);
	addPattern(w, h, state[0], 3, 3,  glider,  0, 40);
	addPattern(w, h, state[0], 9, 50, gosperglidergun, 1, 1);


	// x, y: Posicion actual de la vista
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

		flushinp(); // Evita que quede input almacenado al mantener una tecla
					// apretada y que al soltarla la siga recibiendo
		getmaxyx(stdscr, row, col); // reinica tamano terminal por si se modifico

		//// Realiza una iteracion del juego
		show(w, h, state[i%2], row, col, y, x);
		compute(w, h, state[i%2], state[(i+1)%2]);

		// delay
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