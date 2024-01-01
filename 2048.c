/* Programarea Calculatoarelor, seria CC
 * Tema2 - 2048
 */
 
 // !! Pentru aspectul ideal se recomanda jucarea in fullscreen in terminal !!

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
#define PAUZA 5

/* curses.h este necesar pentru biblioteca grafică ncurses
 * ctype.h este necesar pentru funcția tolower - man tolower pentru detalii
 * string.h este necesar pentru prelucrarea sirurilor de caractere
 * stdlib.h este necesar pentru a putea aloca dinamic
 * time.h este necesar pentru a putea genera numere random
 * unistd.h este necesar pentru optiunea de sleep si pentru a putea lasa
	calculatorul sa "gandeasca" singur o miscare
 */

/* functie pentru initializarea perechilor de culori fg si bg */
void culori(){
	init_pair(1, COLOR_YELLOW, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	init_pair(4, COLOR_BLUE, COLOR_BLACK);
	init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(6, COLOR_CYAN, COLOR_BLACK);
	init_pair(11, COLOR_WHITE, COLOR_BLACK);
}

/* functie care verifica daca matricea este goala / are doar valori = 0 */
int mat_gol(int *mat[]) {
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (mat[i][j] != 0) return 1;
		}
	}
	return 0;
}

/* calculam puterea la care se afla 2 */
int putere(int x) {
	int k = 0;
	while (x > 1) {
		x /= 2;
		k++;
	}
	return k;
}

/* functie de afisare a tablei de joc si a numerelor din matrice */
void afis(int *mat[], WINDOW *joc) {
    int i, j;
	// desenam colturile tablei
	mvwaddch(joc, 1, 6, ACS_ULCORNER);
	mvwaddch(joc, 1, 34, ACS_URCORNER);
	mvwaddch(joc, 9, 6, ACS_LLCORNER);
	mvwaddch(joc, 9, 34, ACS_LRCORNER);
	// desenam restul tablei
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
	// afisam numerele din matrice in culori
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
			if (mat[i][j] == 0) {
				mvwprintw(joc, i * 2 + 2, j * 7 + 8, "    ");
			}
			else {
				wattron(joc, COLOR_PAIR(putere(mat[i][j]) % 6));
            	mvwprintw(joc, i * 2 + 2, j * 7 + 8, "%4d", mat[i][j]);
				wattroff(joc, COLOR_PAIR(putere(mat[i][j]) % 6));
			}
        }
    }
    wrefresh(joc);
}

/* functie care verifica daca jocul a fost castigat, pierdut sau continua */
int win(int *mat[]) {
	int i, j, ok = 1;
	// verificare conditie de castig
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (mat[i][j] == 2048) { 
				return 1;
			}
		}
	}
	// verificare conditii de continuare
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (mat[i][j] == 0) ok = 0;
		}
	}
	// in cazul unei table pline se verifica daca mai sunt mutari posibile
	if (ok == 1) {
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (i == 0) {
					if (j < 3) {
						if (mat[i][j] == mat[i + 1][j] || mat[i][j] == mat[i][j + 1]) {
							ok = 3;
						}
					}
				}
				else if (i == 3) {
					if (j < 3) {
						if (mat[i][j] == mat[i - 1][j] || mat[i][j] == mat[i][j + 1]) {
							ok = 3;
						}
					}
				}
					else if (j == 0) {
						if (i < 3) {
							if (mat[i][j] == mat[i][j + 1] || mat[i][j] == mat[i + 1][j]) {
								ok = 3;
							}
						}
					}
						else if (j == 3) {
							if (i > 0 && i < 3) {
								if(mat[i][j] == mat[i - 1][j] || mat[i][j] == mat[i][j - 1] || mat[i][j] == mat[i + 1][j]) {
									ok = 3;
								}
							}
						}
							else {
								if (mat[i][j] == mat[i - 1][j] || mat[i][j] == mat[i + 1][j]
									|| mat[i][j] == mat[i][j + 1] || mat[i][j] == mat[i][j - 1]) {
									ok = 3;
								} 
							}
			}
		}
	}
	if (ok == 3) return 3;
	if (ok == 1) return 2;
	return 0;
}

/* functie pentru meniu de start */
int menu(WINDOW *wnd, char *variante[], WINDOW *joc, int **mat, WINDOW *scor,int *puncte) {
	int opt, i, high = 0;
	while (FOREVER) {
		wclear(wnd);
		for (i = 0; i < 3; i++) {
			if (i == high) {
				mvwaddch(wnd, i + 19, 85, ACS_LARROW);
				// highlight pe optiunea actuala
				wattron(wnd, A_REVERSE);
				/* in cazul in care nu s-a inceput inca jocul sau acesta a
					fost pierdut optiunea de resume nu va fi evidentiata
					si nici accesibila */
				if (i == 1 && (mat_gol(mat) == 0 || win(mat) == 1 || win(mat) == 2)) {
					wattroff(wnd, A_REVERSE);
				}
			}
			mvwprintw(wnd, i + 19, i + 75, "%s", variante[i]);
			wattroff(wnd, A_REVERSE);
		}
		// deplasare cu sageti in meniu
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
		// selectam cu enter
		if (opt == 10) {
			break;
		}
	}
	return high;
}

/* functie de initializare a matricei de joc si a vectorului de matrici
	folosit pentru undo */
void initmat(int ***mat, int ****undo) {
	int i;
	*mat = (int **)calloc(4, sizeof(int *));
	for (i = 0; i < 4; i++) {
		(*mat)[i] = (int *) calloc (4, sizeof(int));
	}
	*undo = (int ***)calloc(1, sizeof(int **));
	(*undo)[0] = (int **)calloc(4, sizeof(int *));
	for (i = 0; i < 4; i++) {
		(*undo)[0][i] = (int *)calloc(4, sizeof(int));
	}
}

/* functie pentru calculul directiei favorabile
	este utilizata in cazul in care calculatorul decide singur mutarea */
int directie(int *mat[]) {
	int nr = 0, maxim = 0, i, j, dir = 0, k;
	// verificare in dreapta
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 3; j++) {
			k = j + 1;
			while (mat[i][k] == 0 && k < 3) {
				k++;
			}
			if (mat[i][j] == mat[i][k] && mat[i][j] != 0) {
				nr++; 
				j = k;
			}
		}
	}
	if (nr > maxim) {
		maxim = nr;
		dir = 2; 
	}
	// verificare in stanga
	nr = 0;
	for (i = 0; i < 4; i++) {
		for (j = 3; j > 0; j--) {
			k = j - 1;
			while (mat[i][k] == 0 && k > 0) {
				k--;
			}
			if (mat[i][j] == mat[i][k] && mat[i][j] != 0) {
				nr++;
				j = k;
			}
		}
	}
	if (nr > maxim) {
		maxim = nr;
		dir = 3;
	}
	// verificare in sus
	nr = 0;
	for (i = 0; i < 4; i++) {
		for (j = 3; j > 0; j--) {
			k = j - 1;
			while (mat[k][i] == 0 && k > 0) {
				k--;
			}
			if (mat[j][i] == mat[k][i] && mat[j][i] != 0) {
				nr++;
				j = k;
			}
		}
	}
	if (nr > maxim) {
		maxim = nr;
		dir = 0;
	}
	// verificare in jos
	nr = 0;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 3; j++) {
			k = j + 1;
			while (mat[k][i] == 0 && k < 3) {
				k++;
			}
			if (mat[j][i] == mat[k][i] && mat[j][i] !=0) {
				nr++;
				j = k;
			}
		}
	}
	if (nr > maxim) {
		maxim = nr;
		dir = 1;
	}
	// daca nu exista o mutare favorabila, calculatorul va muta random
	if (maxim == 0) {
		dir = rand() % 4;
	}
	return dir;
}

/* functie care plaseaza initial pe pozitii aleatoare 2 numere din multimea
	{2, 4} */
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

/* functie care adauga un element din multimea {2, 4} pe o pozitie aleatoare */
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

/* functie pentru afisarea datei si orei curente */
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

/* functie pentru deplasarea in stanga */
void stanga(int ***mat, int *puncte, int *flag) {
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
			if ((*mat)[i][j] == (*mat)[i][j + 1] && (*mat)[i][j]) {
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
		*flag = 1;
		addrand(&(*mat));
	}
}

/* functie pentru deplasarea in dreapta */
void dreapta(int ***mat, int *puncte, int *flag) {
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
		*flag = 1;
		addrand(&(*mat));
	}
}

/* functie pentru deplasarea in sus */
void sus(int ***mat, int *puncte, int *flag) {
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
			if ((*mat)[j][i] == (*mat)[j + 1][i] && (*mat)[j][i]) {
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
		*flag = 1;
		addrand(&(*mat));
	}
}

/* functie pentru deplasarea in jos */
void jos(int ***mat, int *puncte, int *flag) {
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
			if ((*mat)[j][i] == (*mat)[j - 1][i] && (*mat)[j][i]) {
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
		*flag = 1;
		addrand(&(*mat));
	}
}

/* functie pentru copierea matricei de joc in vectorul de matrici undo */
void copy(int ****undo, int *n, int *mat[]) {
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			(*undo)[*n - 1][i][j] = mat[i][j];
		}
	}
	(*n)++;
	(*undo) = (int ***)realloc(*undo, (*n) * sizeof(int **));
	(*undo)[*n - 1] = (int **)calloc(4, sizeof(int *));
	for (i = 0; i < 4; i++) {
		(*undo)[*n - 1][i] = (int *)calloc(4, sizeof(int));
	}
}

/* functie pentru afisarea statisticilor in timp real */
void statistici(WINDOW *stats, int nr_sus, int nr_jos, int nr_stanga, int nr_dreapta, int nr_auto, int nr_undo) {
	mvwaddstr(stats, 1, 12, "STATS");

	mvwaddstr(stats, 3, 11, "SUS:");
	mvwprintw(stats, 3, 15, "%3d", nr_sus);

	mvwaddstr(stats, 4, 11, "JOS:");
	mvwprintw(stats, 4, 15, "%3d", nr_jos);

	mvwaddstr(stats, 5, 8, "STANGA:");
	mvwprintw(stats, 5, 15, "%3d", nr_stanga);

	mvwaddstr(stats, 6, 7, "DREAPTA:");
	mvwprintw(stats, 6, 15, "%3d", nr_dreapta);

	mvwaddstr(stats, 7, 10, "AUTO:");
	mvwprintw(stats, 7, 15, "%3d", nr_auto);

	mvwaddstr(stats, 8, 10, "UNDO:");
	mvwprintw(stats, 8, 15, "%3d", nr_undo);
	box(stats, 0, 0);
	wrefresh(stats);
}

/* functie pentru optiunea new game din meniu */
void new_game(WINDOW *joc, WINDOW *scor, WINDOW *stats, int *puncte, int ***mat, char com[][15], 
			int *nr_sus, int *nr_jos, int *nr_stanga, int *nr_dreapta, int *nr_auto, int *nr_undo, int ****undo, int *n) {
	int c, k = 4, i, j, dir, ok;
	*nr_sus = *nr_jos = *nr_stanga = *nr_dreapta = *nr_auto = *nr_undo = 0;
	*n = 1;
	WINDOW *castig = initscr();
	for (i = 0; i < 4; i++) {
		strcpy(com[i], "");
	}
	joc = newwin(20, 41, 5, 15);
	scor = newwin(9, 29, 15, 21);
	refresh();
	*puncte = 0;
	initmat(&(*mat), &(*undo));
	start(&(*mat));
	// se atribuie culori ferestrelor
	wattron(joc, COLOR_PAIR(11));
	wattron(scor, COLOR_PAIR(1));
	wattron(stats, COLOR_PAIR(5));
	while (FOREVER) {
		ok = 0;
		// se initiaza asteptarea de 5 secunde a unei miscari
		timeout(1000 * PAUZA);
		afis((*mat), joc);
		wattron(joc, COLOR_PAIR(11));
		statistici(stats, *nr_sus, *nr_jos, *nr_stanga, *nr_dreapta, *nr_auto, *nr_undo);
		mvwprintw(scor, 1, 1, "SCORE: %5d", *puncte);
		wrefresh(scor);
		data(scor);
		// se verifica daca jocul este castigat sau pierdut
		if (win(*mat) == 1) {
			castig = newwin(3, 14, 1, 28);
			wattron(castig, COLOR_PAIR(2));
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
			wattron(castig, COLOR_PAIR(2));
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
		// daca trec 5 secunde calculatorul va face singur o mutare
		if (c == ERR) {
			dir = directie((*mat));
			switch (dir) {
				case 0:
					copy(&(*undo), &(*n), (*mat));
					sus(&(*mat), &(*puncte), &ok);
					if (ok) {
						for (i = k - 1; i > 0; i--) {
							strcpy(com[i], com[i - 1]);
						}
						strcpy(com[0], "SUS_AUTO    ");
					}
					else {
						(*n)--;
					}
					break;
				case 1:
					copy(&(*undo), &(*n), (*mat));
					jos(&(*mat), &(*puncte), &ok);
					if (ok) {
						for(i = k - 1; i > 0; i--) {
							strcpy(com[i], com[i - 1]);
						}
						strcpy(com[0], "JOS_AUTO    ");
					}
					else {
						(*n)--;
					}
					break;
				case 2:
					copy(&(*undo), &(*n), (*mat));
					dreapta(&(*mat), &(*puncte), &ok);
					if (ok) {
						for(i = k - 1; i > 0; i--) {
							strcpy(com[i], com[i - 1]);
						}
						strcpy(com[0], "DREAPTA_AUTO");
					}
					else {
						(*n)--;
					}
					break;
				case 3:
					copy(&(*undo), &(*n), (*mat));
					stanga(&(*mat), &(*puncte), &ok);
					if (ok) {
						for(i = k - 1; i > 0; i--) {
							strcpy(com[i], com[i - 1]);
						}
						strcpy(com[0], "STANGA_AUTO ");
					}
					else {
						(*n)--;
					}
					break;
			}
			for (i = 0; i < k; i++) {
				mvwprintw(scor, i + 4, 5, "%s", com[i]);
			}
			if (ok) {
				(*nr_auto)++;
			}
		}
		else {
			if (c == 'q') {
				clear();
				break;
			}
			else if (c == 'w') {
				copy(&(*undo), &(*n), (*mat));
				sus(&(*mat), &(*puncte), &ok);
				if (ok) {
					(*nr_sus)++;
					for (i = k - 1; i > 0; i--) {
						strcpy(com[i], com[i - 1]);
					}
					strcpy(com[0], "SUS         ");
				}
				else {
					(*n)--;
				}
			}
				else if (c == 's') {
					copy(&(*undo), &(*n), (*mat));
					jos(&(*mat), &(*puncte), &ok);
					if (ok) {
						(*nr_jos)++;
						for(i = k - 1; i > 0; i--) {
							strcpy(com[i], com[i - 1]);
						}
						strcpy(com[0], "JOS         ");		
					}
					else {
						(*n)--;
					}
				}
					else if (c == 'd') {
						copy(&(*undo), &(*n), (*mat));
						dreapta(&(*mat), &(*puncte), &ok);
						if (ok) {
							(*nr_dreapta)++;
							for(i = k - 1; i > 0; i--) {
								strcpy(com[i], com[i - 1]);
							}
							strcpy(com[0], "DREAPTA     ");
						}
						else {
							(*n)--;
						}
					}
						else if (c == 'a') {
							copy(&(*undo), &(*n), (*mat));
							stanga(&(*mat), &(*puncte), &ok);
							if (ok) {
								(*nr_stanga)++;
								for(i = k - 1; i > 0; i--) {
									strcpy(com[i], com[i - 1]);
								}
								strcpy(com[0], "STANGA      ");
							}
							else {
								(*n)--;
							}
						}
							else if (c == 'r') {
								if (*n - 1 > 0) {
									(*nr_undo)++;
									for (i = 0; i < 4; i++) {
										for (j = 0; j < 4; j++) {
											(*mat)[i][j] = (*undo)[*n - 2][i][j];
										}
									}
									(*n)--;
								}
							}
			// afisam mutarile in tabela de scor
			for (i = 0; i < k; i++) {
				mvwprintw(scor, i + 4, 5, "%s", com[i]);
			}
		}
		// resetam asteptarea unei miscari
		timeout(0);
	}
}


/* functie pentru optiunea de resume, similara new_game
	jocul se reia de unde a fost lasat */
void resume(WINDOW *joc, WINDOW *scor, WINDOW *stats, int *puncte, int ***mat, char com[][15],
			int *nr_sus, int *nr_jos, int *nr_stanga, int *nr_dreapta, int *nr_auto, int *nr_undo, int ****undo, int *n) {
	int c, i, j, k = 4, dir, ok;
	WINDOW *castig = initscr();
	joc = newwin(20, 41, 5, 15);
	scor = newwin(9, 29, 15, 21);
	refresh();
	wmove(joc, 5, 5);
	wmove(scor, 1, 1);
	wattron(scor, COLOR_PAIR(1));
	// se afiseaza ultimele mutari
	for (i = 0; i < k; i++) {
			mvwprintw(scor, i + 4, 5, "%s", com[i]);
		}
	while (FOREVER) {
		ok = 0;
		timeout(1000 * PAUZA);
		afis((*mat), joc);
		statistici(stats, *nr_sus, *nr_jos, *nr_stanga, *nr_dreapta, *nr_auto, *nr_undo);
		mvwprintw(scor, 1, 1, "SCORE: %5d", *puncte);
		wattron(scor, COLOR_PAIR(1));
		wrefresh(scor);
		data(scor);
		if (win(*mat) == 1) {
			castig = newwin(3, 14, 1, 28);
			wattron(castig, COLOR_PAIR(2));
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
			wattron(castig, COLOR_PAIR(2));
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
					copy(&(*undo), &(*n), (*mat));
					sus(&(*mat), &(*puncte), &ok);
					if (ok) {
						for (i = k - 1; i > 0; i--) {
							strcpy(com[i], com[i - 1]);
						}
						strcpy(com[0], "SUS_AUTO    ");
					}
					else {
						(*n)--;
					}
					break;
				case 1:
					copy(&(*undo), &(*n), (*mat));
					jos(&(*mat), &(*puncte), &ok);
					if (ok) {
						for(i = k - 1; i > 0; i--) {
							strcpy(com[i], com[i - 1]);
						}
						strcpy(com[0], "JOS_AUTO    ");
					}
					else {
						(*n)--;
					}
					break;
				case 2:
					copy(&(*undo), &(*n), (*mat));
					dreapta(&(*mat), &(*puncte), &ok);
					if (ok) {
						for(i = k - 1; i > 0; i--) {
							strcpy(com[i], com[i - 1]);
						}
						strcpy(com[0], "DREAPTA_AUTO");
					}
					else {
						(*n)--;
					}
					break;
				case 3:
					copy(&(*undo), &(*n), (*mat));
					stanga(&(*mat), &(*puncte), &ok);
					if (ok) {
						for(i = k - 1; i > 0; i--) {
							strcpy(com[i], com[i - 1]);
						}
						strcpy(com[0], "STANGA_AUTO ");
					}
					else {
						(*n)--;
					}
					break;
			}
			for (i = 0; i < k; i++) {
				mvwprintw(scor, i + 4, 5, "%s", com[i]);
			}
			if (ok) {
				(*nr_auto)++;
			}
		}
		else {
			if (c == 'q') {
				clear();
				break;
			}
			else if (c == 'w') {
				copy(&(*undo), &(*n), (*mat));
				sus(&(*mat), &(*puncte), &ok);
				if (ok) {
					(*nr_sus)++;
					for (i = k - 1; i > 0; i--) {
						strcpy(com[i], com[i - 1]);
					}
					strcpy(com[0], "SUS         ");
				}
				else {
					(*n)--;
				}
			}
				else if (c == 's') {
					copy(&(*undo), &(*n), (*mat));
					jos(&(*mat), &(*puncte), &ok);
					if (ok) {
						(*nr_jos)++;
						for(i = k - 1; i > 0; i--) {
							strcpy(com[i], com[i - 1]);
						}
						strcpy(com[0], "JOS         ");
					}
					else {
						(*n)--;
					}
				}
					else if (c == 'd') {
						copy(&(*undo), &(*n), (*mat));
						dreapta(&(*mat), &(*puncte), &ok);
						if (ok) {
							(*nr_dreapta)++;
							for(i = k - 1; i > 0; i--) {
								strcpy(com[i], com[i - 1]);
							}
							strcpy(com[0], "DREAPTA     ");
						}
						else {
							(*n)--;
						}
					}
						else if (c == 'a') {
							copy(&(*undo), &(*n), (*mat));
							stanga(&(*mat), &(*puncte), &ok);
							if (ok) {
								(*nr_stanga)++;
								for(i = k - 1; i > 0; i--) {
									strcpy(com[i], com[i - 1]);
								}
								strcpy(com[0], "STANGA      ");
							}
							else {
								(*n)--;
							}
						}
							else if (c == 'r') {
								if (*n - 1 > 0) {
									(*nr_undo)++;
									for (i = 0; i < 4; i++) {
										for (j = 0; j < 4; j++) {
											(*mat)[i][j] = (*undo)[*n - 2][i][j];
										}
									}
									(*n)--;
								}
							}
			for (i = 0; i < k; i++) {
				mvwprintw(scor, i + 4, 5, "%s", com[i]);
			}
		}
		timeout(0);
	}
}

/* functie pentru tabela lengenda */
void leg(WINDOW *legenda) {
	wattron(legenda, COLOR_PAIR(6));
	mvwaddstr(legenda, 1, 2, "Deplasarea se face cu tastele:");
	mvwaddstr(legenda, 2, 3, "A - Stanga");
	mvwaddstr(legenda, 3, 3, "D - Dreapta");
	mvwaddstr(legenda, 4, 3, "W - Sus");
	mvwaddstr(legenda, 5, 3, "S - Jos");
	box(legenda, 0, 0);
	wrefresh(legenda);
}

int main(void)
{
	int **mat, ***undo;
	int i, x, puncte = 0, n;
	int nr_sus, nr_jos, nr_stanga, nr_dreapta, nr_auto, nr_undo;
	char *variante[] = {"New Game", "Resume", "Quit"};
	char com[4][15];
	for (i = 0; i < 4; i++) {
		strcpy(com[i], "");
	}
	nr_sus = nr_jos = nr_stanga = nr_dreapta = nr_auto = nr_undo = 0;
	//nr de optiuni undo posibile
	n = 1;
	/* Se inițializează ecranul; initscr se va folosi în mod obligatoriu */
	WINDOW *wnd = initscr(), *joc = initscr(), *scor = initscr();
	WINDOW *legenda = initscr(), *stats = initscr();
	initmat(&mat, &undo);
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
	legenda = newwin(7, 34, 5, 70);
	stats = newwin(10, 30, 15, 72);
	/* Se reflectă schimbările pe ecran */
	refresh();
	start_color();
	culori();
	attron(COLOR_PAIR(1));
	/* Rămânem în while până când se primește tasta q */
	while (FOREVER) {
		x = menu(wnd, variante, joc, mat, scor, &puncte);
		switch (x) {
			case 0:
				werase(wnd);
				wrefresh(wnd);
				leg(legenda);
				new_game(joc, scor, stats, &puncte, &mat, com, &nr_sus, &nr_jos, &nr_stanga, &nr_dreapta, &nr_auto, &nr_undo, &undo, &n);
				refresh();
				break;
			case 1:
				if (mat_gol(mat) == 1 && win(mat) != 1 && win(mat) != 2) {
					werase(wnd);
					wrefresh(wnd);
					leg(legenda);
					resume(joc, scor, stats, &puncte, &mat, com, &nr_sus, &nr_jos, &nr_stanga, &nr_dreapta, &nr_auto, &nr_undo, &undo, &n);
				}
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
