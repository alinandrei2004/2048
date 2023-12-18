/* Programarea Calculatoarelor, seria CC
 * Tema2 - 2048
 */
#include <stdio.h>
#include <string.h>
#include <curses.h>
#include <ctype.h>

#define FOREVER 	1
#define INIT_ROW 	20
#define INIT_COL 	20

/* curses.h este necesar pentru biblioteca grafică ncurses
 * ctype.h este necesar pentru funcția tolower - man tolower pentru detalii
 * Inițial, steluța se va afla pe ecran la coordonatele (INIT_ROW, INIT_COL) 
 */

int main(void)
{
	int row = INIT_ROW, col = INIT_COL, new_row, new_col;
	int nrows, ncols;
	char c;
	/* Se inițializează ecranul; initscr se va folosi în mod obligatoriu */
	WINDOW *wnd = initscr();
	
	/* getmaxyx - este un macro, nu o funcție, așă că nu avem adresă la parametri */
	/* Se va reține în nrows și ncols numărul maxim de linii și coloane */
	getmaxyx(wnd, nrows, ncols);

	/* Se șterge ecranul */
	clear();
	/* Se inhibă afișarea caracterelor introduse de la tastatură */
	noecho();
	/* Caracterele introduse sunt citite imediat - fără 'buffering' */
	cbreak();

	/* Se ascunde cursorul */	
	curs_set(0);

	/* Se va afișa un mesaj la poziția formată din primii doi parametri - (par1, par2) */	
	mvaddstr(0, 0, "Puteti sa mutati '*' folosind tastele:");
	mvaddstr(1, 2, "A - stanga");
	mvaddstr(2, 2, "D - dreapta");
	mvaddstr(3, 2, "W - sus");
	mvaddstr(4, 2, "S - jos");
	mvaddstr(5, 0, "Pentru iesire, apasati tasta Q.");

	/* Se mută cursorul la poziția (row, col) */ 
	move(row, col);
	/* Se adaugă la poziția indicată de cursor caracterul '*' */
	addch('*');
	
	/* Se reflectă schimbările pe ecran */
	refresh();

	/* Rămânem în while până când se primește tasta q */
	while (FOREVER) {
		c = getchar();

		if (tolower(c) == 'q') {
			break;
		}
	
		/* Se determină noua poziție, în funcție de tasta apăsată
		 * Nu putem depași nrows și ncols, sau linia 0/coloana 0.
		 */
			
		switch (tolower(c)) {
		case 'a':
			new_row = row;
			if (col > 0)
				new_col = col - 1;
			break;
		
		case 'd':
			new_row = row;
			if (col + 1 < ncols)
				new_col = col + 1;
			break;
		
		case 'w':
			if (row > 0)
				new_row = row - 1;
			new_col = col;
			break;

		case 's':
			if (row + 1 < nrows)
				new_row = row + 1;
			new_col = col;
			break;
		}

		/* Se șterge '*' din vechea poziție */
		move(row, col);
		addch(' ');
		
		/* Se adaugă '*' în noua poziție */
		move(new_row, new_col);
		addch('*');
		refresh();

		row = new_row;
		col = new_col;
	}

	/* Se închide fereastra ncurses */	
	endwin();

	return 0;
}
