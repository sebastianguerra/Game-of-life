#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>


#include "game.h"



void drawScreen(int w, int h, byte matix[w][h], int rows, int columns, int x, int y, int fps, long delta_iter_ms);
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
	{0, 0, 1}, // __O
	{1, 0, 1}, // O_O
	{0, 1, 1}  // _OO
};
byte blinker[3][3] = {
	{0, 1, 0}, // _O_
	{0, 1, 0}, // _O_
	{0, 1, 0}  // _O_
};
byte toad[2][4] = {
	{0, 1, 1, 1}, // _OOO
	{1, 1, 1, 0}, // OOO_
};

/*
________________________O___________
______________________O_O___________
____________OO______OO____________OO
___________O___O____OO____________OO
OO________O_____O___OO______________
OO________O___O_OO____O_O___________
__________O_____O_______O___________
___________O___O____________________
____________OO______________________

*/
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

	int fps = 5;
	int running = 1;



	// row, col: Tamano terminal
	int row, col;
	getmaxyx(stdscr, row, col);

	// w, h: Tamano matriz
	int w = 500, h = 500;

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
	addPattern(w, h, state[0], 9, 50, gosperglidergun, 40, 1);


	// x, y: Posicion actual de la vista
	int x = 0, y = 0;



	struct timespec now, lastFrame, lastIter, delta;
	clock_gettime(CLOCK_REALTIME, &now);
	clock_gettime(CLOCK_REALTIME, &lastIter);
	clock_gettime(CLOCK_REALTIME, &lastFrame);
	long delta_iter_ms, delta_draw_ms;
	long last_iter_ms;

	///////////////////////////////////////////////////////
	//// Loop principal
	///////////////////////////////////////////////////////
	int iter = 0;
	int hasToRedraw = 1;
	for (;;) {
		getmaxyx(stdscr, row, col); // reinica tamano terminal por si se modifico


		clock_gettime(CLOCK_REALTIME, &now);


		sub_timespec(lastFrame, now, &delta);
		timespec2ms(delta, &delta_draw_ms);

		sub_timespec(lastIter, now, &delta);
		timespec2ms(delta, &delta_iter_ms);



		int curr = iter%2;

		int c = getch();
		flushinp(); // Evita que quede input almacenado al mantener una tecla
					// apretada y que al soltarla la siga recibiendo

		switch(c){
			// Opciones de movimiento de pantalla
			case 'a':
				y++;
				hasToRedraw = 1;
				break;
			case 's':
				x--;
				hasToRedraw = 1;
				break;
			case 'w':
				x++;
				hasToRedraw = 1;
				break;
			case 'd':
				y--;
				hasToRedraw = 1;
				break;

			// Opciones para modificar FPS
			case '-':
				fps-=1;
				hasToRedraw = 1;
				break;
			case '+':
				fps+=1;
				hasToRedraw = 1;
				break;
			
			// Pausar/resumir simulacion
			case 'p':
				running = !running;
				hasToRedraw = 1;
				break;
			
			// Dibujar de nuevo
			case 'r':
				hasToRedraw = 1;
				break;

			// Salir
			case 'q':
				goto end_loop;
		}




		// Realiza una iteracion del juego
		if (delta_iter_ms >= 1000/fps) {
			if (running){
				compute(w, h, state[curr], state[!curr]);
				last_iter_ms = delta_iter_ms;
				hasToRedraw = 1;
				iter++;
				lastIter = now;
			}
		}

		if (hasToRedraw && delta_draw_ms >= 20) {
			drawScreen(w, h, state[curr], row, col, x, y, fps, last_iter_ms);
			hasToRedraw = 0;
			lastFrame = now;
		}
		napms(1);
	}
	end_loop:

	endwin();
	return 0;
}


void drawScreen(int w, int h, byte matrix[w][h], int rows, int columns, int x, int y, int fps, long delta_iter_ms) {
	show(w, h, matrix, rows, columns, x, y);
	mvprintw(rows-2, columns-15, "  FPS: %d", fps);
	mvprintw(rows-1, columns-15, "Delta: %ld", delta_iter_ms);
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
