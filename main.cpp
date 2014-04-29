/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: main.cpp
 * Funktion: Hauptprojekt
 * Kommentar: Anpassungen fuer Problembehebung "verlorene input-events", delete primMouse eingefuegt
 * Name: Andreas Dolp
 * Datum: 29.04.2014
 * Version: 0.1
 ---------------------------*/

#include "main.h"
#include "input/inputMouse.h"	/* inputMouse* primMouse = */
#include "output/outputGPIOsysfs.h"	/* outputGPIOsysfs* output = */
#include <cstdio>	/* printf */
#include <unistd.h>	/* sleep */
#include <cstdlib>


int main ( int argc, char* argv[] ) {
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

/* DEKLARATION UND DEFINITION */
	unsigned int iaMyGPIOAddresses[NUM_OF_SIGNALS] = {17,27,22,10,9,11,7};	/* Array der GPIO-Ausgabepins, Reihenfolge XF,XB,YF,YB,ZF,ZB,USBErr */
	bool baMySignalsToSet[NUM_OF_SIGNALS] = {0,0,0,0,0,0,0};	/* Array der zu setzenden Ausgabesignale, wird durch calculate() berechnet */
// TODO Exceptions in main koennen noch nicht gefangen werden, da namespace dann nicht passt!
	inputMouse* primMouse = new inputMouse("/dev/input/event1");	/* Neues inputMouse-Objekt */
	outputGPIOsysfs* GPIOoutput = new outputGPIOsysfs(iaMyGPIOAddresses);	/* Neues outputGPIOsysfs-Objekt */
/* ENDE DER DEKLARATION UND DEFINITION */


/* INITIALISIERUNG */
#ifdef _PRINT
	printf("Starting initialization of GPIOs...\n");
	fflush(stdout);
#endif
	try {
		GPIOoutput->init();
	} catch (int e) {
		if(e > 0) {
			printf("ERROR while initialization! Exception-Codes: %d\n", e);
			return -1;
		}
	}	/* catch */
/* ENDE DER INITIALISIERUNG */

/* WHILE(TRUE)-LOOP */
#ifdef _PRINT
	printf("Entering scanning-loop...\n");
	fflush(stdout);
#endif

	while (1) {
// TODO
		system("clear");
/* LESE EINGABE */
#ifdef _PRINT
//	printf("Reading inputs...\n");
//	fflush(stdout);
#endif
		try {
			primMouse->read();
		} catch (int e) {
			if (e >= EXCEPTION_POLLING_ERROR) {
				printf("ERROR while polling! Exception-Codes: %d\n",e);
				return -1;
			}
		}	/* catch */
/* ENDE DER LESE EINGABE */


// TODO Calculate einbauen
		if(primMouse->getDX() > 0)
			baMySignalsToSet[0] = 1;
		else
			baMySignalsToSet[0] = 0;
		if(primMouse->getDX() < 0)
			baMySignalsToSet[1] = 1;
		else
			baMySignalsToSet[1] = 0;

		if(primMouse->getDY() > 0)
			baMySignalsToSet[2] = 1;
		else
			baMySignalsToSet[2] = 0;
		if(primMouse->getDY() < 0)
			baMySignalsToSet[3] = 1;
		else
			baMySignalsToSet[3] = 0;
		baMySignalsToSet[4] = primMouse->getClickLeft();
		baMySignalsToSet[5] = primMouse->getClickRight();



/* SCHREIBE AUSGABE */
		/* Setze neue Ausgabesignale */
#ifdef _PRINT
//		printf("Setting output-signals...\n");
//		fflush(stdout);
#endif
		try {
			GPIOoutput->setSignals(baMySignalsToSet);
		} catch (int e) {
			if(e > 0) {
				printf("ERROR while setting! Exception-Codes: %d\n", e);
				return -1;
			}
		}	/* catch */

		/* Schreibe Ausgabesignale */
#ifdef _PRINT
//		printf("Writing outputs...\n");
//		fflush(stdout);
#endif
		try {
			GPIOoutput->write();
		} catch (int e) {
			if(e > 0) {
				printf("ERROR while writing! Exception-Codes: %d\n", e);
				return -1;
			}
		}	/* catch */
/* ENDE DER SCHREIBE AUSGABE */

#ifdef _PRINT
		printf("Laufkatze vor:		%d\n",baMySignalsToSet[2]);
		printf("Laufkatze zurück:	%d\n",baMySignalsToSet[3]);
		printf("Schwenkung rechts:	%d\n",baMySignalsToSet[0]);
		printf("Schwenkung links:	%d\n",baMySignalsToSet[1]);
		printf("Haken ab:		%d\n",baMySignalsToSet[4]);
		printf("Haken auf:		%d\n",baMySignalsToSet[5]);
		printf("=========================\n");
		fflush(stdout);
#endif
	}	/* while(1) */
	delete primMouse;
	return 0;
}	/* main() */


