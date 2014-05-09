/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: print.cpp
 * Funktion: Funktion zur Bildschirmausgabe, Programmierung der Funktionen
 * Kommentar: Fehlerverbesserungen
 * Name: Andreas Dolp
 * Datum: 09.05.2014
 * Version: 1.0
 ---------------------------*/

#include "print.h"
#include "../main.h" /* _DEBUG; VERSION */
#include "../output/outputGPIO.h" /* outputGPIO*, SIGNAL_xx */
#include <cstdio> /* printf */
#include <ncurses.h> /* ncurses-Funktionen */
#include <thread> /* thread */
#include <chrono> /* std::chrono::seconds in thread */

/*
 * Funktion zur Initialisierung aller beteiligten Programmfenster
 */
void printInit() {
	int iMainWinMaxX = 0; /* Maximale X-Groesse des Main-Windows */
	int iMainWinMaxY = 0; /* Maximale Y-Groesse des Main-Windows */

/* Initialisiere Main-Window */
	if( (windowpMainWin = initscr()) == NULL ) { /* wenn Fehler, breche ab */
		printf("ERROR: Error creating main-window!\n");
		return;
	}

	curs_set(0); /* Cursor unsichtbar */
	getmaxyx(windowpMainWin, iMainWinMaxY, iMainWinMaxX); /* Ermittle maximale Groesse des Main-Windows */

/* Initialisiere Signal-Window */
	if( (windowpSignalWin = newwin(WIN_SIGNAL_MAX_Y,iMainWinMaxX,WIN_MAIN_NUM_OF_HEADER_LINES,1)) == NULL ) { /* wenn Fehler, breche ab */
		printf("ERROR: Error creating signals-window!\n");
		return;
	}

/* Initialisiere Error-Window */
	if( (windowpErrorWin = newwin(WIN_ERROR_MAX_Y,iMainWinMaxX,iMainWinMaxY-WIN_ERROR_MAX_Y,0)) == NULL ) { /* wenn Fehler, breche ab */
		printf("ERROR: Error creating error-window!\n");
		return;
	}

	scrollok(windowpErrorWin, TRUE); /* Aktiviere Scrolling fuer Error-Fenster */

	if(has_colors()) { /* Wenn aktuelles Terminal Farbdarstellung ermoeglicht */
		start_color(); /* starte Farbdarstellung */

		init_pair(1, COLOR_WHITE, COLOR_BLUE); /* erstelle Farbkombinationen */
		init_pair(2, COLOR_YELLOW, COLOR_RED);

		wbkgd(windowpMainWin, COLOR_PAIR(1)); /* und weise diese den Fenstern zu */
		wbkgd(windowpSignalWin, COLOR_PAIR(1));
		wbkgd(windowpErrorWin, COLOR_PAIR(2));
	}
	wrefresh(windowpErrorWin); /* Schreibe Ausgabe */
	wrefresh(windowpSignalWin); /* Schreibe Ausgabe */
	refresh(); /* Schreibe Ausgabe */
}

/*
 * Funktion zur Ausfuehrung der printSignals() als eigenen Thread
 * @param outputGPIO_curOutput Zeiger auf outputGPIO-Objekt, welche dargestellt werden soll
 */
void printInit_SignalsThread(outputGPIO* outputGPIO_curOutput) {
	while(1) {
		printSignals(outputGPIO_curOutput); /* Rufe printSignals-Funktion auf */
		std::this_thread::sleep_for (std::chrono::seconds(PRINT_SIGNAL_THREAD_REFRESH_TIME)); /* Warte PRINT_SIGNAL_THREAD_REFRESH_TIME-Sekunden bis Aktualisierung */
	}
}

/*
 * Funktion zur Bildschirmausgabe des Titel-Textes im Main-Window
 */
void printTitle() {
	wprintw(windowpMainWin, "ICS - Crane Control\n");
	wprintw(windowpMainWin, "============================================================\n");
	#ifndef _DEBUG
		wprintw(windowpMainWin, "Version %G\n", VERSION);
	#else	/* _DEBUG */
		wprintw(windowpMainWin, "Version %G DEBUG\n", VERSION);
	#endif	/* _DEBUG */
	wprintw(windowpMainWin, "============================================================\n");
	wprintw(windowpMainWin, "Written by Andreas Dolp for ICS - Innovative Crane Solutions\n");
	wprintw(windowpMainWin, "This software is licensed under GNU GPLv3\n");
	wprintw(windowpMainWin, "============================================================\n");
	wprintw(windowpMainWin, "\n");
	refresh(); /* Schreibe Ausgabe */
}

/*
 * Funktion zur Bildschirmausgabe der Signale
 * @param outputGPIO_curOutput Zeiger auf outputGPIO-Objekt, welche dargestellt werden soll
 */
void printSignals(outputGPIO* outputGPIO_curOutput) {
	wclear(windowpSignalWin);
	wprintw(windowpSignalWin, "Schwenkung rechts:	%d\n",outputGPIO_curOutput->getSignal(SIGNAL_YF));
	wprintw(windowpSignalWin, "Schwenkung links:	%d\n",outputGPIO_curOutput->getSignal(SIGNAL_YB));
	wprintw(windowpSignalWin, "Laufkatze vor:		%d\n",outputGPIO_curOutput->getSignal(SIGNAL_XF));
	wprintw(windowpSignalWin, "Laufkatze zurück:	%d\n",outputGPIO_curOutput->getSignal(SIGNAL_XB));
	wprintw(windowpSignalWin, "Haken ab:		%d\n",outputGPIO_curOutput->getSignal(SIGNAL_ZF));
	wprintw(windowpSignalWin, "Haken auf:		%d\n",outputGPIO_curOutput->getSignal(SIGNAL_ZB));
	wprintw(windowpSignalWin, "USB-Fehler aktiv:	%d\n",!outputGPIO_curOutput->getSignal(SIGNAL_USBERR));
	wrefresh(windowpSignalWin);	/* Schreibe Ausgabe */
}

/*
 * Funktion zur Bildschirmausgabe von Fehlermeldungen
 * @param cpOptErrString Optional auszugebender String xx nach ERROR xx, Error-Code: yy, terminiert mit \n
 * @param iOptErrCode Optional auszugebender Fehlercode yy nach ERROR xx, Error-Code: yy
 */
void printError(const char* cpOptErrString, const int iOptErrCode) {
	int iCurCursorPosX = 0; /* Aktuelle X-Position des Cursors */
	int iCurCursorPosY = 0; /* Aktuelle Y-Position des Cursors */
	getyx(windowpErrorWin, iCurCursorPosY, iCurCursorPosX ); /* Ermittle aktuelle Cursor-Position im Error-Fenster */

	if(iCurCursorPosY >= WIN_ERROR_MAX_Y) /* Wenn Cursor-Position in letzter Zeile des Error-Fensters, */
		scroll(windowpErrorWin); /* scrolle Error-Fenster um eine Zeile nach oben */

	if ((cpOptErrString != NULL) && (iOptErrCode == 0)) /* Wenn Fehler mit String OHNE Fehlernummer */
		wprintw(windowpErrorWin, "ERROR %s\n",cpOptErrString);
	if ((cpOptErrString != NULL) && (iOptErrCode != 0)) /* Wenn Fehler mit String UND Fehlernummer */
		wprintw(windowpErrorWin, "ERROR %s	Error-Code: %d\n",cpOptErrString,iOptErrCode);
	if ((cpOptErrString == NULL) && (iOptErrCode == 0)) /* Wenn allgemeiner Fehler */
		wprintw(windowpErrorWin, "ERROR: Undefined Error!\n");
	wrefresh(windowpErrorWin); /* Schreibe Ausgabe */
}


