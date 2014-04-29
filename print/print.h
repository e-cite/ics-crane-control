/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: print.h
 * Funktion: Header zu print.cpp
 * Kommentar: Umbau auf ncurses-print-Funktionen
 * Name: Andreas Dolp
 * Datum: 29.04.2014
 * Version: 0.2
 ---------------------------*/

#ifndef PRINT_H_
#define PRINT_H_

#define WIN_MAIN_NUM_OF_HEADER_LINES 8
#define WIN_SIGNAL_MAX_Y 7
#define WIN_ERROR_MAX_Y 4


#include <ncurses.h>	/* WINDOW * */
#include "../main.h"	/* _DEBUG; VERSION */
#include "../output/outputGPIOsysfs.h"	/* NUM_OF_SIGNALS */

void printInit();
void printTitle();
void printError(const char* cpOptErrString, int iOptErrCode);
void printSignals(const bool baSignals [NUM_OF_SIGNALS]);

#endif /* PRINT_H_ */

#ifdef GLOBAL_WINDOW
	#define EXTERN
#else
	#define EXTERN extern
#endif

EXTERN WINDOW* windowpMainWin;
EXTERN WINDOW* windowpErrorWin;
EXTERN WINDOW* windowpSignalWin;
