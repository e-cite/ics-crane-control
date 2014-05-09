/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: main.cpp
 * Funktion: Hauptprojekt
 * Kommentar: Anpassungen nach Ueberarbeitungen, Erste vollstaendig lauffaehige Version
 * Name: Andreas Dolp
 * Datum: 09.05.2014
 * Version: 1.0
 ---------------------------*/

#include "main.h"
#include "output/outputGPIO.h" /* NUM_OF_SIGNALS */
#include "input/inputMouse.h" /* inputMouse* primMouse = */
#include "input/inputJoystick.h" /* inputJoystick* primJoystick = */
#include "output/outputGPIOsysfs.h" /* outputGPIOsysfs* output = */
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
	unsigned int iaGPIOAddresses[NUM_OF_SIGNALS] = {7,27,22,10,9,11,17}; /* Array der GPIO-Ausgabepins, Reihenfolge USBErr,XF,XB,YF,YB,ZF,ZB; siehe outputGPIO.h */
	bool baSignalsToSet[NUM_OF_SIGNALS] = {0,0,0,0,0,0,0}; /* Array der zu setzenden Ausgabesignale */

	inputMouse* inputMouse_primMouse = NULL; /* Zeiger auf inputMouse-Objekt */
	inputJoystick* inputJoystick_primJoystick = NULL; /* Zeiger auf inputJoystick-Objekt */
	inputMovement* inputMovement_curInputDevice = NULL;  /* Polymorpher Zeiger auf inputMovement-Objekt, gibt aktuell gueltiges Objekt an */
	outputGPIOsysfs* outputGPIOsysfs_RPiGPIO = new outputGPIOsysfs(iaGPIOAddresses); /* Neues outputGPIOsysfs-Objekt */
/* ENDE DER DEKLARATION UND DEFINITION */


/* INITIALISIERUNG */
	printInit(); /* Initialisiere ncurses-Windows */
	printTitle(); /* Gebe Titel aus */
	std::thread threadPrintSignals (printInit_SignalsThread,baSignalsToSet); /* Print-Funktion in eigenem Thread */
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
		if ((inputMouse_primMouse == NULL) && (inputMovement_curInputDevice == NULL)) { /* Wenn kein Maus-Device und kein aktuelles Device vorhanden */
			try {
				inputMouse_primMouse = new inputMouse("/dev/input/by-path/platform-bcm2708_usb-usb-0:1.2:1.1-event-mouse");	/* Neues inputMouse-Objekt */
				inputMovement_curInputDevice = inputMouse_primMouse; /* Weise Adresse des Maus-Objekts dem aktuellen Objekt zu */
			} catch (int e) {
				if (e < 0) { /* Wenn Fehler beim Erstellen des Maus-Objekts auftritt */
					delete inputMouse_primMouse; /* gebe allokierten Speicherplatz wieder frei */
					inputMovement_curInputDevice = NULL; /* Ruecksetze curInputDevice wieder */
					printError("no mouse-device found", e); /* und gebe Fehlermeldung aus */
				}
			}	/* catch */
		}	/* if ((inputMouse_primMouse == NULL) && (inputMovement_curInputDevice == NULL)) */

		if ((inputJoystick_primJoystick == NULL) && (inputMovement_curInputDevice == NULL)) { /* Wenn kein Joystick-Device und kein aktuelles Device vorhanden */
			try {
				inputJoystick_primJoystick = new inputJoystick("/dev/input/by-path/platform-bcm2708_usb-usb-0:1.2:1.0-event-joystick"); /* Neues inputJoystick-Objekt */
				inputMovement_curInputDevice = inputJoystick_primJoystick; /* Weise Adresse des Joystick-Objekts dem aktuellen Objekt zu */
			} catch (int e) {
				if (e > 0) { /* Wenn Fehler beim Erstellen des Joystick-Objekts auftritt */
					delete inputJoystick_primJoystick; /* gebe allokierten Speicherplatz wieder frei */
					inputMovement_curInputDevice = NULL; /* Ruecksetze curInputDevice wieder */
					printError("no joystick-device found", e); /* und gebe Fehlermeldung aus */
				}
			}	/* catch */
		}	/* if ((inputJoystick_primJoystick == NULL) && (inputMovement_curInputDevice == NULL)) */

		if (inputMovement_curInputDevice == NULL) /* Wenn kein Device gefunden, setze USB-Fehler und ruecksetze alle Signale */
			outputGPIOsysfs_RPiGPIO->setUSBErrActive();

/* ENDE DER ERMITTLUNG DES ANGESCHLOSSENEN DEVICES */
		else {

/* Wenn gueltiges Device ermittelt */

/* LESE EINGABE */
			try {
				inputMovement_curInputDevice->read();
			} catch (int e) {
				if (e == EXCEPTION_POLLING_ERROR) {
					printError("while polling input device!", e);
					continue;
				}
				if (e < EXCEPTION_POLLING_ERROR) {
					printError("while reading input device!", e);
					continue;
				}
			}	/* catch */
/* ENDE DER LESE EINGABE */

// TODO Bei Schwellwerten zwischen Maus und Joystick unterscheiden und diese aus config einlesen
/* VERRECHNE WERTE UND MAPPE DIESE AUF AUSGABESIGNALE */
			if(inputMovement_curInputDevice->getDX() > 3)
				baSignalsToSet[SIGNAL_XF] = 1;
			else
				baSignalsToSet[SIGNAL_XF] = 0;
			if(inputMovement_curInputDevice->getDX() < -3)
				baSignalsToSet[SIGNAL_XB] = 1;
			else
				baSignalsToSet[SIGNAL_XB] = 0;

			if(inputMovement_curInputDevice->getDY() > 3)
				baSignalsToSet[SIGNAL_YB] = 1;
			else
				baSignalsToSet[SIGNAL_YB] = 0;
			if(inputMovement_curInputDevice->getDY() < -3)
				baSignalsToSet[SIGNAL_YF] = 1;
			else
				baSignalsToSet[SIGNAL_YF] = 0;

			baSignalsToSet[SIGNAL_ZF] = inputMovement_curInputDevice->getBtn1();
			baSignalsToSet[SIGNAL_ZB] = inputMovement_curInputDevice->getBtn2();
/* ENDE DER VERRECHNE WERTE UND MAPPE DIESE AUF AUSGABESIGNALE */


/* SCHREIBE AUSGABE */
			try {
				outputGPIOsysfs_RPiGPIO->setSignals(baSignalsToSet); /* Setze neue Ausgabesignale */
			} catch (int e) {
				if(e < 0) {
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
	delete inputMouse_primMouse;
	delete inputJoystick_primJoystick;
	delete outputGPIOsysfs_RPiGPIO;
	return 0;
}	/* main() */


