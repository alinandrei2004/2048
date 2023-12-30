/* Programarea Calculatoarelor, seria CC
 * Tema2 - 2048
 */
#include <stdio.h>
#include <string.h>
#include <curses.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define FOREVER 	1
#define INIT_ROW 	20
#define INIT_COL 	20
#define EMPTY 0
#define PAUZA 3

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
			if (mat[i][j] == 0) {
				mvwprintw(joc, i * 2 + 2, j * 7 + 8, "    ");
			}
			else {
            	mvwprintw(joc, i * 2 + 2, j * 7 + 8, "%4d", mat[i][j]);
			}
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

int directie(int *mat[]) {
	int nr = 0, maxim = 0, i, j, dir = 0;
	//dreapta
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 3; j++) {
			if (mat[i][j] == mat[i][j + 1] && mat[i][j] != 0) {
				nr++; 
				j++;
			}
		}
	}
	if (nr > maxim) {
		maxim = nr;
		dir = 2; 
	}
	//stanga
	nr = 0;
	for (i = 0; i < 4; i++) {
		for (j = 3; j > 0; j--) {
			if (mat[i][j] == mat[i][j - 1] && mat[i][j] != 0) {
				nr++;
				j--;
			}
		}
	}
	if (nr > maxim) {
		maxim = nr;
		dir = 3;
	}
	//sus
	nr = 0;
	for (i = 0; i < 4; i++) {
		for (j = 3; j > 0; j--) {
			if (mat[j][i] == mat[j - 1][i] && mat[j][i] != 0) {
				nr++;
				j--;
			}
		}
	}
	if (nr > maxim) {
		maxim = nr;
		dir = 0;
	}
	//jos
	nr = 0;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 3; j++) {
			if (mat[j][i] == mat[j + 1][i] && mat[j][i] !=0) {
				nr++;
				j++;
			}
		}
	}
	if (nr > maxim) {
		maxim = nr;
		dir = 1;
	}
	if (maxim == 0) {
		dir = rand() % 4;
	}
	return dir;
}

int win(int *mat[]) {
	int i, j, ok = 1;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (mat[i][j] == 2048) { 
				return 1;
			}
		}
	}
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (mat[i][j] == 0) ok = 0;
		}
	}
	if (ok) return 2;
	return 0;
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

void addrand(int ***mat) {
	int v[] = {2, 4};
	srand(time(NULL));
	int lin, col;
	lin = rand() % 4;
	col = rand() % 4;
	while ((*mat)[lin][col] != 0) {
		lin = rand() % 4;
		col = rand() % 4;
	}
	(*mat)[lin][col] = v[rand() % 2];
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

void stanga(int ***mat, int *puncte) {
	int i, j, k, ok = 0;
	for (i = 0; i < 4; i++) {
		k = 0;
		for (j = 0; j < 4; j++) {
			if((*mat)[i][j] != 0) {
				(*mat)[i][k] = (*mat)[i][j];
				if(k != j) {
					(*mat)[i][j] = 0;
					ok = 1;
				}
				k++;
			}
		}
	}
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 3; j++) {
			if ((*mat)[i][j] == (*mat)[i][j + 1]) {
				(*mat)[i][j] += (*mat)[i][j + 1];
				(*mat)[i][j + 1] = 0;
				*puncte += (*mat)[i][j];
				ok = 1;
			}
		}
	}
	for (i = 0; i < 4; i++) {
		k = 0;
		for (j = 0; j < 4; j++) {
			if((*mat)[i][j] != 0) {
				(*mat)[i][k] = (*mat)[i][j];
				if(k != j) {
					(*mat)[i][j] = 0;
					ok = 1;
				}
				k++;
			}
		}
	}
	if (ok) {
		addrand(&(*mat));
	}
}

void dreapta(int ***mat, int *puncte) {
	int i, j, k, ok = 0;
	for (i = 0; i < 4; i++) {
		k = 3;
		for (j = 3; j >= 0; j--) {
			if((*mat)[i][j] != 0) {
				(*mat)[i][k] = (*mat)[i][j];
				if(k != j) {
					(*mat)[i][j] = 0;
					ok = 1;
				}
				k--;
			}
		}
	}
	for (i = 0; i < 4; i++) {
		for (j = 3; j > 0; j--) {
			if ((*mat)[i][j] == (*mat)[i][j - 1] && (*mat)[i][j] != 0) {
				(*mat)[i][j] += (*mat)[i][j - 1];
				(*mat)[i][j - 1] = 0;
				*puncte += (*mat)[i][j];
				ok = 1;
			}
		}
	}
	for (i = 0; i < 4; i++) {
		k = 3;
		for (j = 3; j >= 0; j--) {
			if((*mat)[i][j] != 0) {
				(*mat)[i][k] = (*mat)[i][j];
				if(k != j) {
					(*mat)[i][j] = 0;
					ok = 1;
				}
				k--;
			}
		}
	}
	if (ok) {
		addrand(&(*mat));
	}
}

void sus(int ***mat, int *puncte) {
	int i, j, k, ok = 0;
	for (i = 0; i < 4; i++) {
		k = 0;
		for (j = 0; j < 4; j++) {
			if ((*mat)[j][i] != 0) {
				(*mat)[k][i] = (*mat)[j][i];
				if (k != j) {
					(*mat)[j][i] = 0;
					ok = 1;
				}
				k++;
			}
		}
	}
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 3; j++) {
			if ((*mat)[j][i] == (*mat)[j + 1][i]) {
				(*mat)[j][i] += (*mat)[j + 1][i];
				(*mat)[j + 1][i] = 0;
				*puncte += (*mat)[j][i];
				ok = 1;
			}
		}
	}
	for (i = 0; i < 4; i++) {
		k = 0;
		for (j = 0; j < 4; j++) {
			if ((*mat)[j][i] != 0) {
				(*mat)[k][i] = (*mat)[j][i];
				if (k != j) {
					(*mat)[j][i] = 0;
					ok = 1;
				}
				k++;
			}
		}
	}
	if (ok) {
		addrand(&(*mat));
	}
}

void jos(int ***mat, int *puncte) {
	int i, j, k, ok = 0;
	for (i = 0; i < 4; i++) {
		k = 3;
		for (j = 3; j >= 0; j--) {
			if ((*mat)[j][i] != 0) {
				(*mat)[k][i] = (*mat)[j][i];
				if (k != j) {
					(*mat)[j][i] = 0;
					ok = 1;
				} 
				k--;
			}
		}
	}
	for (i = 0; i < 4; i++) {
		for (j = 3; j > 0; j--) {
			if ((*mat)[j][i] == (*mat)[j - 1][i]) {
				(*mat)[j][i] += (*mat)[j - 1][i];
				(*mat)[j - 1][i] = 0;
				*puncte += (*mat)[j][i];
				ok = 1;
			}
		}
	}
	for (i = 0; i < 4; i++) {
		k = 3;
		for (j = 3; j >= 0; j--) {
			if ((*mat)[j][i] != 0) {
				(*mat)[k][i] = (*mat)[j][i];
				if (k != j) {
					(*mat)[j][i] = 0;
					ok = 1;
				} 
				k--;
			}
		}
	}
	if (ok) {
		addrand(&(*mat));
	}
}

void new_game(WINDOW *joc, WINDOW *scor, int *puncte, int ***mat, char com[][15]) {
	int c, k = 4, i, dir;
	WINDOW *castig = initscr();
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
		timeout(1000 * PAUZA);
		afis((*mat), joc);
		mvwprintw(scor, 1, 1, "SCORE: %5d", *puncte);
		wrefresh(scor);
		data(scor);
		if (win(*mat) == 1) {
			castig = newwin(3, 14, 1, 28);
        	box(castig, 0, 0);
        	mvwprintw(castig, 1, 3, "YOU WON!");
        	wrefresh(castig);
			wrefresh(joc);
			wrefresh(scor);
			sleep(5);
			break;
		}
		else if (win(*mat) == 2) {
			castig = newwin(3, 14, 1, 28);
        	box(castig, 0, 0);
        	mvwprintw(castig, 1, 2, "GAME OVER!");
        	wrefresh(castig);
			wrefresh(joc);
			wrefresh(scor);
			sleep(5);
			break;
		}
		box(joc, 0, 0);
		box(scor, 0, 0);
		wrefresh(joc);
		wrefresh(scor);
		c = getch();
		if (c == ERR) {
			dir = directie((*mat));
			switch (dir) {
				case 0:
					sus(&(*mat), &(*puncte));
					for (i = k - 1; i > 0; i--) {
						strcpy(com[i], com[i - 1]);
					}
					strcpy(com[0], "SUS_AUTO    ");
					break;
				case 1:
					jos(&(*mat), &(*puncte));
					for(i = k - 1; i > 0; i--) {
						strcpy(com[i], com[i - 1]);
					}
					strcpy(com[0], "JOS_AUTO    ");
					break;
				case 2:
					dreapta(&(*mat), &(*puncte));
					for(i = k - 1; i > 0; i--) {
						strcpy(com[i], com[i - 1]);
					}
					strcpy(com[0], "DREAPTA_AUTO");
					break;
				case 3:
					stanga(&(*mat), &(*puncte));
					for(i = k - 1; i > 0; i--) {
						strcpy(com[i], com[i - 1]);
					}
					strcpy(com[0], "STANGA_AUTO ");
					break;
			}
			for (i = 0; i < k; i++) {
				mvwprintw(scor, i + 4, 5, "%s", com[i]);
			}
		}
		else {
			if (c == 'q') {
				clear();
				break;
			}
			else if (c == 'w') {
				sus(&(*mat), &(*puncte));
				for (i = k - 1; i > 0; i--) {
					strcpy(com[i], com[i - 1]);
				}
				strcpy(com[0], "SUS     ");
			}
				else if (c == 's') {
					jos(&(*mat), &(*puncte));
					for(i = k - 1; i > 0; i--) {
						strcpy(com[i], com[i - 1]);
					}
					strcpy(com[0], "JOS     ");
				}
					else if (c == 'd') {
						dreapta(&(*mat), &(*puncte));
						for(i = k - 1; i > 0; i--) {
							strcpy(com[i], com[i - 1]);
						}
						strcpy(com[0], "DREAPTA ");
					}
						else if (c == 'a') {
							stanga(&(*mat), &(*puncte));
							for(i = k - 1; i > 0; i--) {
								strcpy(com[i], com[i - 1]);
							}
							strcpy(com[0], "STANGA  ");
						}
			for (i = 0; i < k; i++) {
				mvwprintw(scor, i + 4, 5, "%s", com[i]);
			}
		}
		timeout(0);
	}
}

void resume(WINDOW *joc, WINDOW *scor, int *puncte, int ***mat, char com[][15]) {
	int c, i, k = 4;
	WINDOW *castig = initscr();
	joc = newwin(20, 41, 5, 15);
	scor = newwin(9, 29, 15, 21);
	refresh();
	wmove(joc, 5, 5);
	wmove(scor, 1, 1);
	for (i = 0; i < k; i++) {
			mvwprintw(scor, i + 3, 5, "%s", com[i]);
		}
	while (FOREVER) {
		afis((*mat), joc);
		mvwprintw(scor, 1, 1, "SCORE: %5d", *puncte);
		wrefresh(scor);
		data(scor);
		if (win(*mat) == 1) {
			castig = newwin(3, 14, 1, 28);
        	box(castig, 0, 0);
        	mvwprintw(castig, 1, 3, "YOU WON!");
        	wrefresh(castig);
			wrefresh(joc);
			wrefresh(scor);
			sleep(5);
			break;
		}
		else if (win(*mat) == 2) {
			castig = newwin(3, 14, 1, 28);
        	box(castig, 0, 0);
        	mvwprintw(castig, 1, 2, "GAME OVER!");
        	wrefresh(castig);
			wrefresh(joc);
			wrefresh(scor);
			sleep(5);
			break;
		}
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
			sus(&(*mat), &(*puncte));
			for (i = k - 1; i > 0; i--) {
				strcpy(com[i], com[i - 1]);
			}
			strcpy(com[0], "SUS    ");
		}
			else if (c == 's') {
				jos(&(*mat), &(*puncte));
				for(i = k - 1; i > 0; i--) {
					strcpy(com[i], com[i - 1]);
				}
				strcpy(com[0], "JOS    ");
			}
				else if (c == 'd') {
					dreapta(&(*mat), &(*puncte));
					for(i = k - 1; i > 0; i--) {
						strcpy(com[i], com[i - 1]);
					}
					strcpy(com[0], "DREAPTA");
				}
					else if (c == 'a') {
						stanga(&(*mat), &(*puncte));
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
	char com[4][15];
	for (i = 0; i < 4; i++) {
		strcpy(com[i], "");
	}
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
	
	/* Se reflectă schimbările pe ecran */
	refresh();

	/* Rămânem în while până când se primește tasta q */
	while (FOREVER) {
		x = menu(wnd, variante, joc, mat, scor, &puncte);
		switch (x) {
			case 0:
				werase(wnd);
				wrefresh(wnd);
				new_game(joc, scor, &puncte, &mat, com);
				refresh();
				break;
			case 1:
				werase(wnd);
				wrefresh(wnd);
				resume(joc, scor, &puncte, &mat, com);
				break;
			default:
				endwin();
				return 0;
		}
	}
	/* Se închide fereastra ncurses */	
	endwin();
	return 0;
}
