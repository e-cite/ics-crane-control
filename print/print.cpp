/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: print.cpp
 * Funktion: Funktion zur Bildschirmausgabe, Programmierung der Funktionen
 * Kommentar: Fehlerbehebung bei den Standard-Funktionsparametern der printError-Funktion
 * Name: Andreas Dolp
 * Datum: 06.05.2014
 * Version: 0.3
 ---------------------------*/

#include "print.h"
#include <cstdio>	/* printf */
#include "../main.h"	/* _DEBUG; VERSION */
#include "../output/outputGPIOsysfs.h"	/* NUM_OF_SIGNALS */
#include <ncurses.h>	/* ncurses-Funktionen */
#include <thread>	/* thread */
#include <chrono>	/* std::chrono::seconds in thread */

/* INITIALISIERUNG */
void printInit() {
	int iMainWinMaxX = 0;
	int iMainWinMaxY = 0;

	/* Initialisiere Haupt-Fenster, wenn Fehler breche ab */
	if( (windowpMainWin = initscr()) == NULL ) {
		printf("ERROR: Error creating main-window!\n");
		return;
	}
	curs_set(0);	/* Cursor unsichtbar */
	getmaxyx(windowpMainWin, iMainWinMaxY, iMainWinMaxX);	/* Ermittle maximale Groesse des Haupt-Fensters */
	/* Initialisiere Signal-Fenster, wenn Fehler breche ab */
	if( (windowpSignalWin = newwin(WIN_SIGNAL_MAX_Y,iMainWinMaxX,WIN_MAIN_NUM_OF_HEADER_LINES,1)) == NULL ) {
		printf("ERROR: Error creating signals-window!\n");
		return;
	}

	/* Initialisiere Error-Fenster, wenn Fehler breche ab */
	if( (windowpErrorWin = newwin(WIN_ERROR_MAX_Y,iMainWinMaxX,iMainWinMaxY-WIN_ERROR_MAX_Y,0)) == NULL ) {
		printf("ERROR: Error creating error-window!\n");
		return;
	}
	scrollok(windowpErrorWin, TRUE);	/* Aktiviere Scrolling fuer Error-Fenster */

	/* Wenn aktuelles Terminal Farbdarstellung ermoeglicht */
	if(has_colors()) {
		/* starte Farbdarstellung */
		start_color();
		/* erstelle Farbkombinationen */
		init_pair(1, COLOR_WHITE, COLOR_BLUE);
		init_pair(2, COLOR_YELLOW, COLOR_RED);
		/* und weise diese den Fenstern zu */
		wbkgd(windowpMainWin, COLOR_PAIR(1));
		wbkgd(windowpSignalWin, COLOR_PAIR(1));
		wbkgd(windowpErrorWin, COLOR_PAIR(2));
	}
	wrefresh(windowpErrorWin);	/* Schreibe Ausgabe */
	wrefresh(windowpSignalWin);	/* Schreibe Ausgabe */
	refresh();	/* Schreibe Ausgabe */
}

/* SIGNAL-AUSGABE IM THREAD */
void printInit_SignalsThread(const bool baSignals [NUM_OF_SIGNALS]) {
	while(1) {
		printSignals(baSignals);	/* Rufe printSignals-Funktion auf */
		std::this_thread::sleep_for (std::chrono::seconds(PRINT_SIGNAL_THREAD_REFRESH_TIME)); /* Warte PRINT_SIGNAL_THREAD_REFRESH_TIME-Sekunden bis Aktualisierung */
	}
}

/* TITEL-AUSGABE */
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
	refresh();	/* Schreibe Ausgabe */
}


/* SIGNAL-AUSGABE */
void printSignals(const bool baSignals [NUM_OF_SIGNALS]) {
	wclear(windowpSignalWin);
	wprintw(windowpSignalWin, "Schwenkung rechts:	%d\n",baSignals[0]);
	wprintw(windowpSignalWin, "Schwenkung links:	%d\n",baSignals[1]);
	wprintw(windowpSignalWin, "Laufkatze vor:		%d\n",baSignals[2]);
	wprintw(windowpSignalWin, "Laufkatze zurück:	%d\n",baSignals[3]);
	wprintw(windowpSignalWin, "Haken ab:		%d\n",baSignals[4]);
	wprintw(windowpSignalWin, "Haken auf:		%d\n",baSignals[5]);
	wprintw(windowpSignalWin, "USB-Fehler aktiv:	%d\n",!baSignals[6]);
	wrefresh(windowpSignalWin);	/* Schreibe Ausgabe */
}


/* FEHLER-AUSGABE */
void printError(const char* cpOptErrString = 0, const int iOptErrCode) {
	int iCurCursorPosX = 0;
	int iCurCursorPosY = 0;
	getyx(windowpErrorWin, iCurCursorPosY, iCurCursorPosX );	/* Ermittle aktuelle Cursor-Position im Error-Fenster */
	/* Wenn Cursor-Position in letzter Zeile des Error-Fensters, scrolle Error-Fenster um eine Zeile nach oben */
	if(iCurCursorPosY >= WIN_ERROR_MAX_Y)
		scroll(windowpErrorWin);

	/* FEHLER MIT FEHLERNUMMER */
	if (cpOptErrString != 0 && iOptErrCode != 0) {
		wprintw(windowpErrorWin, "ERROR %s	Error-Code: %d\n",cpOptErrString,iOptErrCode);
	} else {	/* ALLGEMEINER FEHLER */
		wprintw(windowpErrorWin, "ERROR: Undefined Error! Goodbye...\n");
	}
	wrefresh(windowpErrorWin);	/* Schreibe Ausgabe */
}


