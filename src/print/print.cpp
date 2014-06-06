/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: print.cpp
 * Funktion: Funktion zur Bildschirmausgabe, Programmierung der Funktionen
 * Kommentar: Aenderungen in der Versionsausgabe in der Titelzeile
 * Name: Andreas Dolp
 * Datum: 05.06.2014
 * Version: 1.3
 ---------------------------*/

#include "print.h"
#include "../main.h" /* VERSION */
#include "../output/outputGPIO.h" /* outputGPIO*, SIGNAL_xx */
#include <cstdio> /* printf */
#ifdef DEBUG
#ifdef NCURSES
#include <ncurses.h> /* ncurses-Funktionen */
#endif /* NCURSES */
#include <thread> /* thread */
#include <chrono> /* std::chrono::seconds in thread */
#endif /* DEBUG */

/*
 * Funktion zur Initialisierung aller beteiligten Programmfenster
 */
void printInit() {
#ifdef NCURSES
	int iMainWinMaxX = 0; /* Maximale X-Groesse des Main-Windows */
	int iMainWinMaxY = 0; /* Maximale Y-Groesse des Main-Windows */

/* Initialisiere Main-Window */
	if( (windowpMainWin = initscr()) == 0 ) { /* wenn Fehler, breche ab */
		printf("ERROR: Error creating main-window!\n");
		return;
	}

	curs_set(0); /* Cursor unsichtbar */
	getmaxyx(windowpMainWin, iMainWinMaxY, iMainWinMaxX); /* Ermittle maximale Groesse des Main-Windows */

/* Initialisiere Signal-Window */
	if( (windowpSignalWin = newwin(WIN_SIGNAL_MAX_Y,iMainWinMaxX,WIN_MAIN_NUM_OF_HEADER_LINES,1)) == 0 ) { /* wenn Fehler, breche ab */
		printf("ERROR: Error creating signals-window!\n");
		return;
	}

/* Initialisiere Error-Window */
	if( (windowpErrorWin = newwin(WIN_ERROR_MAX_Y,iMainWinMaxX,iMainWinMaxY-WIN_ERROR_MAX_Y,0)) == 0 ) { /* wenn Fehler, breche ab */
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
#endif /* NCURSES */
}

/*
 * Funktion zur Ausfuehrung der printSignals() als eigenen Thread
 * @param outputGPIO_curOutput Zeiger auf outputGPIO-Objekt, welche dargestellt werden soll
 */
void printInit_SignalsThread(outputGPIO* outputGPIO_curOutput) {
#ifdef DEBUG
	while(1) {
		printSignals(outputGPIO_curOutput); /* Rufe printSignals-Funktion auf */
		std::this_thread::sleep_for (std::chrono::seconds(PRINT_SIGNAL_THREAD_REFRESH_TIME)); /* Warte PRINT_SIGNAL_THREAD_REFRESH_TIME-Sekunden bis Aktualisierung */
	}
#endif /* DEBUG */
}

/*
 * Funktion zur Bildschirmausgabe des Titel-Textes im Main-Window
 */
void printTitle() {
#ifdef NCURSES
	wprintw(windowpMainWin, "ICS - Crane Control\n");
	wprintw(windowpMainWin, "============================================================\n");
	wprintw(windowpMainWin, "Version %s\n", VERSION);
	wprintw(windowpMainWin, "============================================================\n");
	wprintw(windowpMainWin, "Written by Andreas Dolp for ICS - Innovative Crane Solutions\n");
	wprintw(windowpMainWin, "This software is licensed under GNU GPLv3\n");
	wprintw(windowpMainWin, "============================================================\n");
	wprintw(windowpMainWin, "\n");
	refresh(); /* Schreibe Ausgabe */
#else /* NCURSES */
	printf("ICS - Crane Control\n");
	printf("Version %s\n", VERSION);
	printf("============================================================\n");
	printf("Written by Andreas Dolp for ICS - Innovative Crane Solutions\n");
	printf("This software is licensed under GNU GPLv3\n");
	printf("============================================================\n");
	printf("\n");
	fflush(stdout); /* Schreibe Ausgabe */
#endif /* NCURSES */
}

/*
 * Funktion zur Bildschirmausgabe der Signale
 * @param outputGPIO_curOutput Zeiger auf outputGPIO-Objekt, welche dargestellt werden soll
 */
void printSignals(outputGPIO* outputGPIO_curOutput) {
#ifdef NCURSES
	wclear(windowpSignalWin);
	wprintw(windowpSignalWin, "Schwenkung rechts:	%d\n",outputGPIO_curOutput->getSignal(SIGNAL_YF));
	wprintw(windowpSignalWin, "Schwenkung links:	%d\n",outputGPIO_curOutput->getSignal(SIGNAL_YB));
	wprintw(windowpSignalWin, "Laufkatze vor:		%d\n",outputGPIO_curOutput->getSignal(SIGNAL_XF));
	wprintw(windowpSignalWin, "Laufkatze zurück:	%d\n",outputGPIO_curOutput->getSignal(SIGNAL_XB));
	wprintw(windowpSignalWin, "Haken ab:		%d\n",outputGPIO_curOutput->getSignal(SIGNAL_ZF));
	wprintw(windowpSignalWin, "Haken auf:		%d\n",outputGPIO_curOutput->getSignal(SIGNAL_ZB));
	wprintw(windowpSignalWin, "USB-Fehler aktiv:	%d\n",!outputGPIO_curOutput->getSignal(SIGNAL_USBERR));
	wrefresh(windowpSignalWin);	/* Schreibe Ausgabe */
#else /* NCURSES */
	printf("Schwenkung rechts:	%d\n",outputGPIO_curOutput->getSignal(SIGNAL_YF));
	printf("Schwenkung links:	%d\n",outputGPIO_curOutput->getSignal(SIGNAL_YB));
	printf("Laufkatze vor:		%d\n",outputGPIO_curOutput->getSignal(SIGNAL_XF));
	printf("Laufkatze zurück:	%d\n",outputGPIO_curOutput->getSignal(SIGNAL_XB));
	printf("Haken ab:		%d\n",outputGPIO_curOutput->getSignal(SIGNAL_ZF));
	printf("Haken auf:		%d\n",outputGPIO_curOutput->getSignal(SIGNAL_ZB));
	printf("USB-Fehler aktiv:	%d\n",!outputGPIO_curOutput->getSignal(SIGNAL_USBERR));
	printf("============================================================\n");
	fflush(stdout); /* Schreibe Ausgabe */
#endif /* NCURSES */
}

/*
 * Funktion zur Bildschirmausgabe von Fehlermeldungen
 * @param cpOptErrString Optional auszugebender String xx nach ERROR xx, Error-Code: yy, terminiert mit \n
 * @param iOptErrCode Optional auszugebender Fehlercode yy nach ERROR xx, Error-Code: yy
 */
void printError(const char* cpOptErrString, const int iOptErrCode) {
#ifdef NCURSES
	int iCurCursorPosX = 0; /* Aktuelle X-Position des Cursors */
	int iCurCursorPosY = 0; /* Aktuelle Y-Position des Cursors */
	getyx(windowpErrorWin, iCurCursorPosY, iCurCursorPosX ); /* Ermittle aktuelle Cursor-Position im Error-Fenster */

	if(iCurCursorPosY >= WIN_ERROR_MAX_Y) /* Wenn Cursor-Position in letzter Zeile des Error-Fensters, */
		scroll(windowpErrorWin); /* scrolle Error-Fenster um eine Zeile nach oben */

	if ((cpOptErrString != 0) && (iOptErrCode == 0)) /* Wenn Fehler mit String OHNE Fehlernummer */
		wprintw(windowpErrorWin, "ERROR %s\n",cpOptErrString);
	if ((cpOptErrString != 0) && (iOptErrCode != 0)) /* Wenn Fehler mit String UND Fehlernummer */
		wprintw(windowpErrorWin, "ERROR %s	Error-Code: %d\n",cpOptErrString,iOptErrCode);
	if ((cpOptErrString == 0) && (iOptErrCode == 0)) /* Wenn allgemeiner Fehler */
		wprintw(windowpErrorWin, "ERROR: Undefined Error!\n");
	wrefresh(windowpErrorWin); /* Schreibe Ausgabe */
#else
	if ((cpOptErrString != 0) && (iOptErrCode == 0)) /* Wenn Fehler mit String OHNE Fehlernummer */
		printf("ERROR %s\n",cpOptErrString);
	if ((cpOptErrString != 0) && (iOptErrCode != 0)) /* Wenn Fehler mit String UND Fehlernummer */
		printf("ERROR %s	Error-Code: %d\n",cpOptErrString,iOptErrCode);
	if ((cpOptErrString == 0) && (iOptErrCode == 0)) /* Wenn allgemeiner Fehler */
		printf("ERROR: Undefined Error!\n");
	printf("============================================================\n");
	fflush(stdout); /* Schreibe Ausgabe */
#endif /* NCURSES */
}


