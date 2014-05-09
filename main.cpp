/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: main.cpp
 * Funktion: Hauptprojekt
 * Kommentar: Fehlerverbesserungen IN ALLEN DATEIEN
 * Name: Andreas Dolp
 * Datum: 09.05.2014
 * Version: 1.0
 ---------------------------*/

#include "main.h"
#include "output/outputGPIO.h" /* NUM_OF_SIGNALS, SIGNAL_xx */
#include "input/inputMouse.h" /* new inputMouse */
#include "input/inputJoystick.h" /* new inputJoystick */
#include "output/outputGPIOsysfs.h" /* outputGPIOsysfs* outputGPIOsysfs_RPiGPIO = new outputGPIOsysfs */
#define GLOBAL_WINDOW /* UNBEDINGT ERFORDERLICH FUER GLOBALE NCURSES-VARIABLEN */
#include "print/print.h" /* print() */
#include <cstdio> /* printf */
#include <thread> /* thread */

/*
 * main.cpp - int main ( int argc, char* argv[] )
 * Hauptfunktion zur Steuerung aller beteiligten Programmteile
 * @return 0 bei korrekter Ausfuehrung, Exception-Code der Unterfunktionen (negativ) bei Abbruch durch kritischen Fehler
 */
int main ( int argc, char* argv[] ) {
/* DEKLARATION UND DEFINITION */
	unsigned int iaGPIOAddresses[NUM_OF_SIGNALS] = {7,17,27,22,10,9,11}; /* Array der GPIO-Ausgabepins, Reihenfolge USBErr,XF,XB,YF,YB,ZF,ZB; siehe outputGPIO.h */
	bool baSignalsToSet[NUM_OF_SIGNALS] = {0,0,0,0,0,0,0}; /* Array der zu setzenden Ausgabesignale */

	inputMovement* inputMovement_curInputDevice = NULL; /* Polymorpher Zeiger auf inputMovement-Objekt, gibt aktuell gueltiges Objekt an */
	outputGPIOsysfs* outputGPIOsysfs_RPiGPIO = new outputGPIOsysfs(iaGPIOAddresses); /* Neues outputGPIOsysfs-Objekt */
/* ENDE DER DEKLARATION UND DEFINITION */


/* INITIALISIERUNG */
	printInit(); /* Initialisiere ncurses-Windows */
	printTitle(); /* Gebe Titel aus */
	std::thread threadPrintSignals (printInit_SignalsThread,outputGPIOsysfs_RPiGPIO); /* Print-Funktion in eigenem Thread */
	try {
		outputGPIOsysfs_RPiGPIO->init(); /* Initialisiere GPIO-Ausgaenge */
	} catch (int e) {
		if(e < 0) { /* Wenn kritischer Fehler, breche ab */
			printError("while GPIO initialization!", e);
			return e;
		}
	} /* catch */
/* ENDE DER INITIALISIERUNG */


/* WHILE(TRUE)-LOOP */
	while (1) {
/* ERMITTLUNG DES ANGESCHLOSSENEN DEVICES */
		if (inputMovement_curInputDevice == NULL) { /* Wenn kein aktuelles Device vorhanden */
			try {
				inputMovement_curInputDevice = new inputMouse("/dev/input/by-path/platform-bcm2708_usb-usb-0:1.2:1.1-event-mouse");	/* Neues inputMouse-Objekt */
			} catch (int e) {
				if (e < 0) { /* Wenn Fehler beim Erstellen des Maus-Objekts auftritt */
					delete inputMovement_curInputDevice; /* gebe allokierten Speicherplatz wieder frei */
					inputMovement_curInputDevice = NULL; /* und Ruecksetze curInputDevice wieder */
				}
			}	/* catch */
		}	/* if (inputMovement_curInputDevice == NULL) */

		if (inputMovement_curInputDevice == NULL) { /* Wenn kein aktuelles Device vorhanden */
			try {
				inputMovement_curInputDevice = new inputJoystick("/dev/input/by-path/platform-bcm2708_usb-usb-0:1.2:1.0-event-joystick"); /* Neues inputJoystick-Objekt */
			} catch (int e) {
				if (e > 0) { /* Wenn Fehler beim Erstellen des Joystick-Objekts auftritt */
					delete inputMovement_curInputDevice; /* gebe allokierten Speicherplatz wieder frei */
					inputMovement_curInputDevice = NULL; /* und Ruecksetze curInputDevice wieder */
				}
			}	/* catch */
		}	/* if (inputMovement_curInputDevice == NULL) */

		if (inputMovement_curInputDevice == NULL) { /* Wenn kein Device gefunden */
			outputGPIOsysfs_RPiGPIO->setUSBErrActive(); /* setze USB-Fehler, ruecksetze alle Signale */
			printError("no input device found"); /* und gebe Warnung aus */
		}

/* ENDE DER ERMITTLUNG DES ANGESCHLOSSENEN DEVICES */
		else {

/* AB HIER GEULTIGES DEVICE ERMITTELT */
			baSignalsToSet[SIGNAL_USBERR] = !GPIO_USBERROR_ACTIVE_STATE; /* Da gueltiges Device, Ruecksetze USB-Fehler */

/* LESE EINGABE */
			try {
				inputMovement_curInputDevice->read();
			} catch (int e) {
				if (e == EXCEPTION_POLLING_ERROR) { /* Im Fehlerfall */
					printError("while polling input device!", e); /* gebe Fehlermeldung aus */
					inputMovement_curInputDevice = NULL; /* setze Zeiger auf aktuelles Device zurueck */
					continue; /* und beginne Schleife von vorne */
				}
				if (e < EXCEPTION_POLLING_ERROR) { /* Im Fehlerfall */
					printError("while reading input device!", e); /* gebe Fehlermeldung aus */
					inputMovement_curInputDevice = NULL; /* setze Zeiger auf aktuelles Device zurueck */
					continue; /* und beginne Schleife von vorne */
				}
			}	/* catch */
/* ENDE DER LESE EINGABE */

// TODO Bei Schwellwerten zwischen Maus und Joystick unterscheiden und diese aus config einlesen
/* VERRECHNE WERTE UND MAPPE DIESE AUF AUSGABESIGNALE */
			if(inputMovement_curInputDevice->getDX() > 3)
				baSignalsToSet[SIGNAL_YF] = 1;
			else
				baSignalsToSet[SIGNAL_YF] = 0;
			if(inputMovement_curInputDevice->getDX() < -3)
				baSignalsToSet[SIGNAL_YB] = 1;
			else
				baSignalsToSet[SIGNAL_YB] = 0;

			if(inputMovement_curInputDevice->getDY() > 3)
				baSignalsToSet[SIGNAL_XB] = 1;
			else
				baSignalsToSet[SIGNAL_XB] = 0;
			if(inputMovement_curInputDevice->getDY() < -3)
				baSignalsToSet[SIGNAL_XF] = 1;
			else
				baSignalsToSet[SIGNAL_XF] = 0;

			baSignalsToSet[SIGNAL_ZF] = inputMovement_curInputDevice->getBtn1();
			baSignalsToSet[SIGNAL_ZB] = inputMovement_curInputDevice->getBtn2();
/* ENDE DER VERRECHNE WERTE UND MAPPE DIESE AUF AUSGABESIGNALE */


/* SCHREIBE AUSGABE */
			try {
				outputGPIOsysfs_RPiGPIO->setSignals(baSignalsToSet); /* Setze neue Ausgabesignale */
			} catch (int e) {
				if(e == EXCEPTION_INCONSISTENT_SIGNALS_TO_SET) {
					printError("while setting signals!", e);
				}
			}	/* catch */

		} /* if (inputMovement_curInputDevice == NULL) ... else */


		try {
			outputGPIOsysfs_RPiGPIO->write(); /* Schreibe Ausgabesignale */
		} catch (int e) {
			if(e < 0) { /* Wenn kritischer Fehler, breche ab */
				printError("while writing GPIO!", e);
				return e;
			}
		} /* catch */
/* ENDE DER SCHREIBE AUSGABE */

	}	/* while(1) */

	/* Gebe Speicherplatz der Heap-Objekte wieder frei */
	delete inputMovement_curInputDevice;
	delete outputGPIOsysfs_RPiGPIO;
	return 0;
}	/* main() */


