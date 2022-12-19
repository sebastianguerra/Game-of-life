#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>


#include "game.h"



void drawScreen(int w, int h, byte matix[w][h], int rows, int columns, int x, int y, int fps, long delta_ms);
void show (int w, int h, byte matrix[w][h], int rows, int columns, int x, int y);











enum { NS_PER_S  = 1000000000,
	   NS_PER_MS = 1000000,
	   MS_PER_S  = 1000 };

void sub_timespec(struct timespec t1, struct timespec t2, struct timespec *td)
{
    td->tv_nsec = t2.tv_nsec - t1.tv_nsec;
    td->tv_sec  = t2.tv_sec - t1.tv_sec;
    if (td->tv_sec > 0 && td->tv_nsec < 0)
    {
        td->tv_nsec += NS_PER_S;
        td->tv_sec--;
    }
    else if (td->tv_sec < 0 && td->tv_nsec > 0)
    {
        td->tv_nsec -= NS_PER_S;
        td->tv_sec++;
    }
}

void timespec2ms(struct timespec t, long *r) {
	*r = t.tv_nsec/NS_PER_MS;
	*r += t.tv_sec*MS_PER_S;
}








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

	int fps = 60;
	int running = 1;



	// row, col: Tamano terminal
	int row, col;
	getmaxyx(stdscr, row, col);

	// w, h: Tamano matriz
	int w = row, h = col;

	// state: Pareja de matrices de tamano WxH que almacenan informacion
	// 		  del estado actual del juego. 
	//		  Se usa una pareja para alternar la matriz en la que se escribira 
	//		  la informacion en cada turno.
	byte state[2][h][w];
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



	struct timespec now, lastFrame, delta;
	clock_gettime(CLOCK_REALTIME, &now);
	clock_gettime(CLOCK_REALTIME, &lastFrame);

	///////////////////////////////////////////////////////
	//// Loop principal
	///////////////////////////////////////////////////////
	int iter = 0;
	for (;;) {
		clock_gettime(CLOCK_REALTIME, &now);

		sub_timespec(lastFrame, now, &delta);
		long delta_ms;
		timespec2ms(delta, &delta_ms);

		int curr = iter%2;

		switch(getch()){
			// Opciones de movimiento de pantalla
			case 'h':
				y++;
				goto REDRAW;
			case 'j':
				x--;
				goto REDRAW;
			case 'k':
				x++;
				goto REDRAW;
			case 'l':
				y--;
				goto REDRAW;

			// Opciones para modificar FPS
			case '-':
				fps-=1;
				goto REDRAW;
			case '+':
				fps+=1;
				goto REDRAW;
			
			// Pausar/resumir simulacion
			case 'p':
				running = !running;
				goto REDRAW;
			
			// Dibujar de nuevo
			case 'r':
				goto REDRAW;

			// Salir
			case 'q':
				goto end_loop;
		}

		if(0) {
			REDRAW:
			drawScreen(w, h, state[curr], row, col, x, y, fps, delta_ms);
		}

		flushinp(); // Evita que quede input almacenado al mantener una tecla
					// apretada y que al soltarla la siga recibiendo
		getmaxyx(stdscr, row, col); // reinica tamano terminal por si se modifico

		// Realiza una iteracion del juego
		if (delta_ms >= 1000/fps) {
			if (running){
				drawScreen(w, h, state[curr], row, col, x, y, fps, delta_ms);
				compute(w, h, state[curr], state[!curr]);
				iter++;
				lastFrame = now;
			}
		}

		// delay
		napms(1);
	}

	end_loop:

	endwin();
	return 0;
}


void drawScreen(int w, int h, byte matrix[w][h], int rows, int columns, int x, int y, int fps, long delta_ms) {
	show(w, h, matrix, rows, columns, x, y);
	mvprintw(rows-2, columns-15, "  FPS: %d", fps);
	mvprintw(rows-1, columns-15, "Delta: %ld", delta_ms);
	refresh();

}


void show (int w, int h, byte matrix[w][h], int rows, int columns, int x, int y) {
	clear();
	for(int i = 0; i < w; i++) {
		for(int j = 0; j < h; j++) {
			char r;

			// el quinto bit indica el estado de la celula
			r = (matrix[i][j] >> 4) ? 'O' : '.';

			mvaddch(i+x, j+y, r);
		}
	}
	refresh();
}