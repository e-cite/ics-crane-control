/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: main.cpp
 * Funktion: Hauptprojekt
 * Kommentar: Anpassungen zum Umbau auf ncurses-print-Funktionen
 * Name: Andreas Dolp
 * Datum: 28.04.2014
 * Version: 0.1
 ---------------------------*/

#include "main.h"
#include "input/inputMouse.h"	/* inputMouse* primMouse = */
#include "output/outputGPIOsysfs.h"	/* outputGPIOsysfs* output = */
#define GLOBAL_WINDOW	/* UNBEDINGT ERFORDERLICH FUER GLOBALE NCURSES-VARIABLEN */
#include "print/print.h"	/* print() */
#include <cstdio>	/* printf */
#include <unistd.h>	/* sleep */
#include <cstdlib>


int main ( int argc, char* argv[] ) {

/* DEKLARATION UND DEFINITION */
	unsigned int iaMyGPIOAddresses[NUM_OF_SIGNALS] = {17,27,22,10,9,11,7};	/* Array der GPIO-Ausgabepins, Reihenfolge XF,XB,YF,YB,ZF,ZB,USBErr */
	bool baMySignalsToSet[NUM_OF_SIGNALS] = {0,0,0,0,0,0,0};	/* Array der zu setzenden Ausgabesignale, wird durch calculate() berechnet */
	inputMouse* primMouse = new inputMouse("/dev/input/event1");	/* Neues inputMouse-Objekt */
	outputGPIOsysfs* GPIOoutput = new outputGPIOsysfs(iaMyGPIOAddresses);	/* Neues outputGPIOsysfs-Objekt */
/* ENDE DER DEKLARATION UND DEFINITION */


/* INITIALISIERUNG */
	printInit();
	printTitle();
	try {
		GPIOoutput->init();
	} catch (int e) {
		if(e > 0) {
			printError("while GPIO initialization!", e);
			return -1;
		}
	}	/* catch */
/* ENDE DER INITIALISIERUNG */

/* WHILE(TRUE)-LOOP */

	while (1) {
/* LESE EINGABE */
		try {
			primMouse->read();
		} catch (int e) {
			if (e >= EXCEPTION_POLLING_ERROR) {
				printError("while polling input device!", e);
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
			baMySignalsToSet[3] = 1;
		else
			baMySignalsToSet[3] = 0;
		if(primMouse->getDY() < 0)
			baMySignalsToSet[2] = 1;
		else
			baMySignalsToSet[2] = 0;
		baMySignalsToSet[4] = primMouse->getClickLeft();
		baMySignalsToSet[5] = primMouse->getClickRight();



/* SCHREIBE AUSGABE */
		/* Setze neue Ausgabesignale */
		try {
			GPIOoutput->setSignals(baMySignalsToSet);
		} catch (int e) {
			if(e > 0) {
				printError("while setting signals!", e);
// TODO Fehlerbehandlung bei falschen setting-signals				return -1;
			}
		}	/* catch */

		/* Schreibe Ausgabesignale */
		try {
			GPIOoutput->write();
		} catch (int e) {
			if(e > 0) {
				printError("while writing output signals!",e);
				return -1;
			}
		}	/* catch */
/* ENDE DER SCHREIBE AUSGABE */

		printSignals(baMySignalsToSet);
	}	/* while(1) */
	return 0;
}	/* main() */


