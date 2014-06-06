/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: print.h
 * Funktion: Header zu print.cpp, Deklaration der fuer die Bildschirmausgabe erforderlichen Funktionen
 * Kommentar: Einbau von printf-Ausgabe bei nicht definiertem Flag NCURSES
 * Name: Andreas Dolp
 * Datum: 27.05.2014
 * Version: 1.2
 ---------------------------*/

#ifndef PRINT_H_
#define PRINT_H_

#define PRINT_SIGNAL_THREAD_REFRESH_TIME 1

#ifdef NCURSES
#define WIN_MAIN_NUM_OF_HEADER_LINES 8
#define WIN_SIGNAL_MAX_Y 7
#define WIN_ERROR_MAX_Y 4

#include <ncurses.h> /* WINDOW * */
#endif /* NCURSES */

#include "../output/outputGPIO.h" /* outputGPIO* */

void printInit(); /* Funktion zur Initialisierung aller beteiligten Programmfenster */
void printInit_SignalsThread(outputGPIO*); /* Funktion zur Ausfuehrung der printSignals() als eigenen Thread */
void printTitle(); /* Funktion zur Bildschirmausgabe des Titel-Fensters */
void printSignals(outputGPIO*); /* Funktion zur Bildschirmausgabe der Signale */
void printError(const char* = 0, int = 0); /* Funktion zur Bildschirmausgabe von Fehlermeldungen */

#endif /* PRINT_H_ */

#ifdef NCURSES
#ifdef GLOBAL_WINDOW
	#define EXTERN
#else
	#define EXTERN extern
#endif

EXTERN WINDOW* windowpMainWin; /* Globaler Zeiger auf Main-Window */
EXTERN WINDOW* windowpErrorWin; /* Globaler Zeiger auf Error-Window */
EXTERN WINDOW* windowpSignalWin; /* Globaler Zeiger auf Signal-Window */
#endif /* NCURSES */
