/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: print.cpp
 * Funktion: Funktion zur Bildschirmausgabe, Programmierung der Funktionen
 * Kommentar: Erste Version
 * Name: Andreas Dolp
 * Datum: 26.04.2014
 * Version: 0.1
 ---------------------------*/

#include "print.h"
#include <cstdio>	/* printf */
#include <cstdlib>	/* system */
#include "../main.h"	/* _DEBUG; VERSION */
#include "../output/outputGPIOsysfs.h"	/* NUM_OF_SIGNALS */

/* TITEL-DIALOG */
void printTitle() {
		system("clear");
		printf("ICS - Crane Control\n");
		printf("============================================================\n");
		#ifndef _DEBUG
			printf("Version %G\n", VERSION);
		#else	/* _DEBUG */
			printf("Version %G DEBUG\n", VERSION);
		#endif	/* _DEBUG */
		printf("============================================================\n");
		printf("Written by Andreas Dolp for ICS - Innovative Crane Solutions\n");
		printf("This software is licensed under GNU GPLv3\n");
		printf("============================================================\n");
		fflush(stdout);
}


/* SIGNAL-AUSGABE */
void printSignals(const bool baSignals [NUM_OF_SIGNALS]) {
	printf("Schwenkung rechts:	%d\n",baSignals[0]);
	printf("Schwenkung links:	%d\n",baSignals[1]);
	printf("Laufkatze vor:		%d\n",baSignals[2]);
	printf("Laufkatze zurück:	%d\n",baSignals[3]);
	printf("Haken ab:		%d\n",baSignals[4]);
	printf("Haken auf:		%d\n",baSignals[5]);
	printf("USB-Fehler aktiv:	%d\n",!baSignals[6]);
	printf("=========================\n");
	fflush(stdout);
}


/* STANDARD-AUSGABE */
void print(const bool baSignals [NUM_OF_SIGNALS], const bool printTitle = false) {
	if(printTitle)
		::printTitle();
	printSignals(baSignals);
	fflush(stdout);
}


/* FEHLERDIALOG */
void printError(const char* cpOptErrString = 0, const int iOptErrCode = 0) {
	/* FEHLER MIT FEHLERNUMMER */
	if (cpOptErrString != 0 && iOptErrCode != 0) {
			printf("ERROR: %s,	Error-Code: %d\n",cpOptErrString,iOptErrCode);
	} else {	/* ALLGEMEINER FEHLER */
		printf("ERROR: Undefined Error! Goodbye...\n");
	}
	fflush(stdout);
}


