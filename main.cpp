/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: main.cpp
 * Funktion: Hauptprojekt
 * Kommentar: Anpassungen fuer inputJoystick
 * Name: Andreas Dolp
 * Datum: 07.05.2014
 * Version: 0.1
 ---------------------------*/

#include "main.h"
#include "input/inputMouse.h"	/* inputMouse* primMouse = */
#include "input/inputJoystick.h"	/* inputJoystick* primJoystick = */
#include "output/outputGPIOsysfs.h"	/* outputGPIOsysfs* output = */
#define GLOBAL_WINDOW	/* UNBEDINGT ERFORDERLICH FUER GLOBALE NCURSES-VARIABLEN */
#include "print/print.h"	/* print() */
#include <cstdio>	/* printf */
#include <unistd.h>	/* sleep */
#include <cstdlib>
#include <thread>	/* thread */


int main ( int argc, char* argv[] ) {

/* DEKLARATION UND DEFINITION */
	unsigned int iaMyGPIOAddresses[NUM_OF_SIGNALS] = {17,27,22,10,9,11,7};	/* Array der GPIO-Ausgabepins, Reihenfolge XF,XB,YF,YB,ZF,ZB,USBErr */
	bool baMySignalsToSet[NUM_OF_SIGNALS] = {0,0,0,0,0,0,0};	/* Array der zu setzenden Ausgabesignale, wird durch calculate() berechnet */
	inputMouse* inputMouse_primMouse = NULL;
	inputJoystick* inputJoystick_primJoystick = NULL;
	inputMovement* inputMovement_curInputDevice = NULL;
	outputGPIOsysfs* GPIOoutput = new outputGPIOsysfs(iaMyGPIOAddresses);	/* Neues outputGPIOsysfs-Objekt */
/* ENDE DER DEKLARATION UND DEFINITION */


/* INITIALISIERUNG */
	printInit();	/* Initialisiere ncurses-Windows */
	printTitle();	/* Gebe Titel aus */
	std::thread threadPrintSignals (printInit_SignalsThread,baMySignalsToSet); /* Print-Funktion in eigenem Thread */
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
/* ERMITTLUNG DES ANGESCHLOSSENEN DEVICES */
		/* Wenn kein Maus-Device und kein aktuelles Device vorhanden */
		if (inputMouse_primMouse == NULL && inputMovement_curInputDevice == NULL) {
			try {
				inputMouse_primMouse = new inputMouse("/dev/input/by-path/platform-bcm2708_usb-usb-0:1.2:1.1-event-mouse");	/* Neues inputMouse-Objekt */
			} catch (int e) {
				if (e > 0) {	/* Wenn Fehler beim Erstellen des Maus-Objekts auftritt */
					delete inputMouse_primMouse;	/* gebe allokierten Speicherplatz wieder frei */
					printError("no mouse-device found", e);	/* und gebe Fehlermeldung aus */
				} else {	/* Wenn Erstellen fehlerfrei ablaeuft */
					inputMovement_curInputDevice = inputMouse_primMouse;	/* Weise Adresse des Maus-Objekts dem aktuellen Objekt zu */
				}
			}	/* catch */
		}	/* if (inputMouse_primMouse == NULL && inputMovement_curInputDevice == NULL) */

		/* Wenn kein Joystick-Device und kein aktuelles Device vorhanden */
		if (inputJoystick_primJoystick == NULL && inputMovement_curInputDevice == NULL) {
			try {
				inputJoystick_primJoystick = new inputJoystick("/dev/input/by-path/platform-bcm2708_usb-usb-0:1.2:1.0-event-joystick");	/* Neues inputJoystick-Objekt */
			} catch (int e) {
				if (e > 0) {	/* Wenn Fehler beim Erstellen des Joystick-Objekts auftritt */
					delete inputJoystick_primJoystick;	/* gebe allokierten Speicherplatz wieder frei */
					printError("no joystick-device found", e);	/* und gebe Fehlermeldung aus */
				} else {	/* Wenn Erstellen fehlerfrei ablaeuft */
					inputMovement_curInputDevice = inputJoystick_primJoystick;	/* Weise Adresse des Joystick-Objekts dem aktuellen Objekt zu */
				}
			}	/* catch */
		}	/* if (inputJoystick_primJoystick == NULL && inputMovement_curInputDevice == NULL) */

// TODO wenn beide Devices nicht erkannt, schreibe USB-Fehler
/* ENDE DER ERMITTLUNG DES ANGESCHLOSSENEN DEVICES */



/* LESE EINGABE */
			try {
				inputMovement_curInputDevice->read();
			} catch (int e) {
// TODO EXCEPTIONS STANDARDISIEREN
				if (e >= EXCEPTION_POLLING_ERROR) {
					printError("while polling input device!", e);
					return -1;
				}
			}	/* catch */
/* ENDE DER LESE EINGABE */


// TODO Calculate einbauen
			if(inputMovement_curInputDevice->getDX() > 1)
				baMySignalsToSet[0] = 1;
			else
				baMySignalsToSet[0] = 0;
			if(inputMovement_curInputDevice->getDX() < -1)
				baMySignalsToSet[1] = 1;
			else
				baMySignalsToSet[1] = 0;

			if(inputMovement_curInputDevice->getDY() > 1)
				baMySignalsToSet[3] = 1;
			else
				baMySignalsToSet[3] = 0;
			if(inputMovement_curInputDevice->getDY() < -1)
				baMySignalsToSet[2] = 1;
			else
				baMySignalsToSet[2] = 0;
			baMySignalsToSet[4] = inputMovement_curInputDevice->getBtn1();
			baMySignalsToSet[5] = inputMovement_curInputDevice->getBtn2();



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

	/* Gebe Speicherplatz der Heap-Objekte wieder frei */
	delete inputMouse_primMouse;
	delete inputJoystick_primJoystick;
	return 0;
}	/* main() */


