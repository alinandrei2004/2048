/* Programarea Calculatoarelor, seria CC
 * Tema2 - 2048
 */
#include <stdio.h>
#include <string.h>
#include <curses.h>
#include <ctype.h>
#include <stdlib.h>

#define FOREVER 	1
#define INIT_ROW 	20
#define INIT_COL 	20
#define EMPTY 0

/* curses.h este necesar pentru biblioteca grafică ncurses
 * ctype.h este necesar pentru funcția tolower - man tolower pentru detalii
 * Inițial, steluța se va afla pe ecran la coordonatele (INIT_ROW, INIT_COL) 
 */
 
void afis(int *mat[], WINDOW *joc) {
    int i, j;
	mvwaddch(joc, 1, 2, ACS_ULCORNER);
	mvwaddch(joc, 1, 30, ACS_URCORNER);
	mvwaddch(joc, 9, 2, ACS_LLCORNER);
	mvwaddch(joc, 9, 30, ACS_LRCORNER);
	for (i = 1; i < 11; i += 2) {
		for (j = 3; j < 30; j++) {
			if (j == 9 || j == 16 || j == 23) {
				if (i == 1) {
					mvwaddch(joc, i, j, ACS_TTEE);
				}
				else if (i == 9) {
					mvwaddch(joc, i, j, ACS_BTEE);
				}
			}
			else {
				mvwaddch(joc, i, j, ACS_HLINE);
			}
		}
	}
	for (i = 2; i < 9; i++) {
		for (j = 2; j < 31; j += 7) {
			if (i == 3 || i == 5 || i == 7) {
				if (j == 2) {
					mvwaddch(joc, i, j, ACS_LTEE);
				}
				else if (j == 30) {
					mvwaddch(joc, i, j, ACS_RTEE);
				}
				else {
					mvwaddch(joc, i, j, ACS_PLUS);
				}
			}
			else {
				mvwaddch(joc, i, j, ACS_VLINE);
			}
		}
	}
	
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            mvwprintw(joc, i * 2 + 2, j * 7 + 4, "%4d", mat[i][j]);
        }
    }
    wrefresh(joc);
}


int menu(WINDOW *wnd, char *variante[], WINDOW *joc, int **mat, WINDOW *scor,int *puncte) {
	int opt, i, high = 0;
	while (FOREVER) {
		wclear(wnd);
		for (i = 0; i < 3; i++) {
			if (i == high) {
				wattron(wnd, A_REVERSE);
			}
			mvwprintw(wnd, i + 1, 1, "%s", variante[i]);
			wattroff(wnd, A_REVERSE);
		}
		opt = wgetch(wnd);
		switch (opt) {
			case KEY_UP:
				high--;
				if (high == -1) {
					high = 0;
				}
				break;
			case KEY_DOWN:
				high++;
				if (high == 3) {
					high = 2;
				}
				break;
			default:
				break;
		}
		if (opt == 10) {
			break;
		}
	}
	return high;
}

void new_game(WINDOW *joc, WINDOW *scor, int *puncte, int **mat) {
	int c;
	while (FOREVER) {
		joc = newwin(20,40, 5, 15);
		scor = newwin(5, 10, 15, 40);
		refresh();
		*puncte = 0;
		wmove(joc, 5, 5);
		wmove(scor, 1, 1);
		//matrice noua 
		afis(mat, joc);
		box(joc, 0, 0);
		box(scor, 0, 0);
		wrefresh(joc);
		wrefresh(scor);
		c = getch();
		if (c == 'q') {
			clear();
			break;
		}
	}
}

void resume(WINDOW *joc, WINDOW *scor, int *puncte, int **mat) {
	int c;
	while (FOREVER) {
		joc = newwin(20,40, 7, 15);
		refresh();
		wmove(joc, 5, 5);
		afis(mat, joc);
		box(joc, 0, 0);
		wrefresh(joc);
		c = getch();
		if (c == 'q') {
			clear();
			break;
		}
	}
}

int main(void)
{
	int **mat = (int **)calloc(4, sizeof(int *));
	int row = INIT_ROW, col = INIT_COL, new_row, new_col;
	int nrows, ncols, i, j, x, puncte = 0;
	char c, *variante[] = {"New Game", "Resume", "Quit"};
	for (i = 0; i < 4; i++) {
		mat[i] = (int *) calloc (4, sizeof(int));
	}
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			mat[i][j] = 2048;
		}
	}
	/* Se inițializează ecranul; initscr se va folosi în mod obligatoriu */
	WINDOW *wnd = initscr(), *joc = initscr(), *scor = initscr();

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

	//folosim sageti pentru deplasare
	keypad(wnd, true);
	
	/* Se va afișa un mesaj la poziția formată din primii doi parametri - (par1, par2) */	
	/*mvaddstr(0, 0, "Puteti sa mutati '*' folosind tastele:");
	mvaddstr(1, 2, "A - stanga");
	mvaddstr(2, 2, "D - dreapta");
	mvaddstr(3, 2, "W - sus");
	mvaddstr(4, 2, "S - jos");
	mvaddstr(5, 0, "Pentru iesire, apasati tasta Q.");*/

	/* Se mută cursorul la poziția (row, col) */ 
	//  move(row, col);
	/* Se adaugă la poziția indicată de cursor caracterul '*' */
	//  addch('*');
	
	/* Se reflectă schimbările pe ecran */
	refresh();

	/* Rămânem în while până când se primește tasta q */
	while (FOREVER) {
		x = menu(wnd, variante, joc, mat, scor, &puncte);
		switch (x) {
			case 0:
				werase(wnd);
				wrefresh(wnd);
				new_game(joc, scor, &puncte, mat);
				refresh();
				break;
			case 1:
				werase(wnd);
				wrefresh(wnd);
				resume(joc, scor, &puncte, mat);
				break;
			default:
				endwin();
				return 0;
		}
		/*c = getchar();

		if (tolower(c) == 'q') {
			break;
		}*/
	
		/* Se determină noua poziție, în funcție de tasta apăsată
		 * Nu putem depași nrows și ncols, sau linia 0/coloana 0.
		 */
			
		/*switch (tolower(c)) {
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
		/*move(row, col);
		addch(' ');
		
		/* Se adaugă '*' în noua poziție */
		/*move(new_row, new_col);
		addch('*');
		refresh();

		row = new_row;
		col = new_col;*/
	}

	/* Se închide fereastra ncurses */	
	endwin();

	return 0;
}
