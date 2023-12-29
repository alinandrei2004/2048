/* Programarea Calculatoarelor, seria CC
 * Tema2 - 2048
 */
#include <stdio.h>
#include <string.h>
#include <curses.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

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
	mvwaddch(joc, 1, 6, ACS_ULCORNER);
	mvwaddch(joc, 1, 34, ACS_URCORNER);
	mvwaddch(joc, 9, 6, ACS_LLCORNER);
	mvwaddch(joc, 9, 34, ACS_LRCORNER);
	for (i = 1; i < 11; i += 2) {
		for (j = 7; j < 34; j++) {
			if (j == 13 || j == 20 || j == 27) {
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
		for (j = 6; j < 37; j += 7) {
			if (i == 3 || i == 5 || i == 7) {
				if (j == 6) {
					mvwaddch(joc, i, j, ACS_LTEE);
				}
				else if (j == 34) {
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
            mvwprintw(joc, i * 2 + 2, j * 7 + 8, "%4d", mat[i][j]);
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

void initmat(int ***mat) {
	int i;
	*mat = (int **)calloc(4, sizeof(int *));
	for (i = 0; i < 4; i++) {
		(*mat)[i] = (int *) calloc (4, sizeof(int));
	}
}

void start(int ***mat) {
	int v[] = {2, 4};
	srand(time(NULL));
	int lin1, col1, lin2, col2;
	lin1 = rand() % 4;
	col1 = rand() % 4;
	lin2 = rand() % 4;
	col2 = rand() % 4;
	while (lin1 == lin2 && col1 == col2) {
		lin2 = rand() % 4;
		col2 = rand() % 4;
	}
	(*mat)[lin1][col1] = v[rand() % 2];
	(*mat)[lin2][col2] = v[rand() % 2];
}

void data(WINDOW *scor) {
	time_t aux;
	char sir[50];
	struct tm *timp;
	time(&aux);
	timp = localtime(&aux);
	strftime(sir, sizeof(sir), "%d-%m-%Y", timp);
	mvwprintw(scor, 1, 15, "%s", sir);
	strftime(sir, sizeof(sir), "%H:%M:%S", timp);
	mvwprintw(scor, 2, 15, "%s", sir);
}

void new_game(WINDOW *joc, WINDOW *scor, int *puncte, int ***mat) {
	int c, k = 4, i;
	char com[4][15];
	for (i = 0; i < 4; i++) {
		strcpy(com[i], "");
	}
	joc = newwin(20, 41, 5, 15);
	scor = newwin(9, 29, 15, 21);
	refresh();
	*puncte = 0;
	wmove(joc, 5, 5);
	wmove(scor, 1, 1);
	initmat(&(*mat));
	start(&(*mat));
	while (FOREVER) {
		afis((*mat), joc);
		mvwprintw(scor, 1, 1, "SCORE: %5d", *puncte);
		wrefresh(scor);
		data(scor);
		box(joc, 0, 0);
		box(scor, 0, 0);
		wrefresh(joc);
		wrefresh(scor);
		c = getch();
		if (c == 'q') {
			clear();
			break;
		}
		else if (c == 'w') {
			for (i = k - 1; i > 0; i--) {
				strcpy(com[i], com[i - 1]);
			}
			strcpy(com[0], "SUS    ");
		}
			else if (c == 's') {
				for(i = k - 1; i > 0; i--) {
					strcpy(com[i], com[i - 1]);
				}
				strcpy(com[0], "JOS    ");
			}
				else if (c == 'd') {
					for(i = k - 1; i > 0; i--) {
						strcpy(com[i], com[i - 1]);
					}
					strcpy(com[0], "DREAPTA");
				}
					else if (c == 'a') {
						for(i = k - 1; i > 0; i--) {
							strcpy(com[i], com[i - 1]);
						}
						strcpy(com[0], "STANGA ");
					}
		for (i = 0; i < k; i++) {
			mvwprintw(scor, i + 3, 5, "%s", com[i]);
		}
	}
}

void resume(WINDOW *joc, WINDOW *scor, int *puncte, int ***mat) {
	int c, i, k = 4;
	char com[4][15];
	for (i = 0; i < 4; i++) {
		strcpy(com[i], "");
	}
	joc = newwin(20, 41, 5, 15);
	scor = newwin(9, 29, 15, 21);
	refresh();
	wmove(joc, 5, 5);
	wmove(scor, 1, 1);
	while (FOREVER) {
		afis((*mat), joc);
		mvwprintw(scor, 1, 1, "SCORE: %5d", *puncte);
		wrefresh(scor);
		data(scor);
		box(joc, 0, 0);
		box(scor, 0, 0);
		wrefresh(joc);
		wrefresh(scor);
		c = getch();
		if (c == 'q') {
			clear();
			break;
		}
		else if (c == 'w') {
			for (i = k - 1; i > 0; i--) {
				strcpy(com[i], com[i - 1]);
			}
			strcpy(com[0], "SUS    ");
		}
			else if (c == 's') {
				for(i = k - 1; i > 0; i--) {
					strcpy(com[i], com[i - 1]);
				}
				strcpy(com[0], "JOS    ");
			}
				else if (c == 'd') {
					for(i = k - 1; i > 0; i--) {
						strcpy(com[i], com[i - 1]);
					}
					strcpy(com[0], "DREAPTA");
				}
					else if (c == 'a') {
						for(i = k - 1; i > 0; i--) {
							strcpy(com[i], com[i - 1]);
						}
						strcpy(com[0], "STANGA ");
					}
		for (i = 0; i < k; i++) {
			mvwprintw(scor, i + 3, 5, "%s", com[i]);
		}
	}
}

int main(void)
{
	int **mat;
	int row = INIT_ROW, col = INIT_COL, new_row, new_col;
	int nrows, ncols, i, j, x, puncte = 0;
	char c, *variante[] = {"New Game", "Resume", "Quit"};
	/* Se inițializează ecranul; initscr se va folosi în mod obligatoriu */
	WINDOW *wnd = initscr(), *joc = initscr(), *scor = initscr();
	initmat(&mat);
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
				new_game(joc, scor, &puncte, &mat);
				refresh();
				break;
			case 1:
				werase(wnd);
				wrefresh(wnd);
				resume(joc, scor, &puncte, &mat);
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
