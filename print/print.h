/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: print.h
 * Funktion: Header zu print.cpp, Definition der fuer die Bildschirmausgabe erforderlichen Funktionen
 * Kommentar: Fehlerverbesserungen
 * Name: Andreas Dolp
 * Datum: 09.05.2014
 * Version: 1.0
 ---------------------------*/

#ifndef PRINT_H_
#define PRINT_H_

#define WIN_MAIN_NUM_OF_HEADER_LINES 8
#define WIN_SIGNAL_MAX_Y 7
#define WIN_ERROR_MAX_Y 4

#define PRINT_SIGNAL_THREAD_REFRESH_TIME 1

#include <ncurses.h> /* WINDOW * */
#include "../main.h" /* _DEBUG; VERSION */
#include "../output/outputGPIO.h" /* outputGPIO* */

void printInit(); /* Funktion zur Initialisierung aller beteiligten Programmfenster */
void printInit_SignalsThread(outputGPIO*); /* Funktion zur Ausfuehrung der printSignals() als eigenen Thread */
void printTitle(); /* Funktion zur Bildschirmausgabe des Titel-Fensters */
void printSignals(outputGPIO*); /* Funktion zur Bildschirmausgabe der Signale */
void printError(const char* = NULL, int = 0); /* Funktion zur Bildschirmausgabe von Fehlermeldungen */

#endif /* PRINT_H_ */

#ifdef GLOBAL_WINDOW
	#define EXTERN
#else
	#define EXTERN extern
#endif

EXTERN WINDOW* windowpMainWin; /* Globaler Zeiger auf Main-Window */
EXTERN WINDOW* windowpErrorWin; /* Globaler Zeiger auf Error-Window */
EXTERN WINDOW* windowpSignalWin; /* Globaler Zeiger auf Signal-Window */
