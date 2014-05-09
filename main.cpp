/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: main.cpp
 * Funktion: Hauptprojekt
 * Kommentar: Anpassungen nach Ueberarbeitungen
 * Name: Andreas Dolp
 * Datum: 08.05.2014
 * Version: 0.1
 ---------------------------*/

#include "main.h"
#include "input/inputMouse.h"	/* inputMouse* primMouse = */
#include "input/inputJoystick.h"	/* inputJoystick* primJoystick = */
#include "output/outputGPIOsysfs.h"	/* outputGPIOsysfs* output = */
#define GLOBAL_WINDOW	/* UNBEDINGT ERFORDERLICH FUER GLOBALE NCURSES-VARIABLEN */
#include "print/print.h"	/* print() */
#include <cstdlib>
#include <thread>	/* thread */

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
	std::thread threadPrintSignals (printInit_SignalsThread,baMySignalsToSet);
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
			baMySignalsToSet[SIGNAL_XF] = 1;
		else
			baMySignalsToSet[SIGNAL_XF] = 0;
		if(primMouse->getDX() < 0)
			baMySignalsToSet[SIGNAL_XB] = 1;
		else
			baMySignalsToSet[SIGNAL_XB] = 0;

		if(primMouse->getDY() > 0)
			baMySignalsToSet[SIGNAL_YB] = 1;
		else
			baMySignalsToSet[SIGNAL_YB] = 0;
		if(primMouse->getDY() < 0)
			baMySignalsToSet[SIGNAL_YF] = 1;
		else
			baMySignalsToSet[SIGNAL_YF] = 0;
		baMySignalsToSet[SIGNAL_ZF] = primMouse->getBtn1();
		baMySignalsToSet[SIGNAL_ZB] = primMouse->getBtn2();

		baMySignalsToSet[SIGNAL_USBERR] = 1;



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

	}	/* while(1) */

	delete(GPIOoutput);
    delete primMouse;
//    delete primJoystick;
	return 0;
}	/* main() */


