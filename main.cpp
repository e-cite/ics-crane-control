/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: main.cpp
 * Funktion: Hauptprojekt
 * Kommentar: Einbau der readConfig-Funktion
 * Name: Andreas Dolp
 * Datum: 23.05.2014
 * Version: 1.1
 ---------------------------*/

#include "main.h"
#include "readConfig/readConfig.h"
#include "searchDevice/searchDevicePath.h" /* searchDevicePath */
#include "output/outputGPIO.h" /* NUM_OF_SIGNALS, SIGNAL_xx */
#include "input/inputMouse.h" /* new inputMouse */
#include "input/inputJoystick.h" /* new inputJoystick */
#include "output/outputGPIOsysfs.h" /* outputGPIOsysfs* outputGPIOsysfs_RPiGPIO = new outputGPIOsysfs */
#define GLOBAL_WINDOW /* UNBEDINGT ERFORDERLICH FUER GLOBALE NCURSES-VARIABLEN */
#include "print/print.h" /* print() */
#include <thread> /* thread */

/*
 * @brief Hauptfunktion zur Steuerung aller beteiligten Programmteile
 * @return 0 bei korrekter Ausfuehrung, Exception-Code der Unterfunktionen (negativ) bei Abbruch durch kritischen Fehler
 */
int main ( int argc, char* argv[] ) {
/* DEKLARATION UND DEFINITION */
	int iCurExceptionCode = 0; /* Enthaelt aktuellen ExceptionCode */
	configValues* configValuespConfigData = new (configValues); /* struct configValues, enthaelt die eingelesenen Konfigurationsdaten */
//TODO Fehlerbehandlung readConfig
	readConfig(configValuespConfigData,"config.ini");
	unsigned int iaGPIOAddresses[NUM_OF_SIGNALS] = {
		configValuespConfigData->iGpioUSBError,
		configValuespConfigData->iGpioXF,
		configValuespConfigData->iGpioXB,
		configValuespConfigData->iGpioYF,
		configValuespConfigData->iGpioYB,
		configValuespConfigData->iGpioZF,
		configValuespConfigData->iGpioZB,
	}; /* Array der GPIO-Ausgabepins, Reihenfolge USBErr,XF,XB,YF,YB,ZF,ZB; siehe outputGPIO.h */
	bool baSignalsToSet[NUM_OF_SIGNALS] = {!GPIO_SIGNAL_ACTIVE_STATE,!GPIO_SIGNAL_ACTIVE_STATE,!GPIO_SIGNAL_ACTIVE_STATE,!GPIO_SIGNAL_ACTIVE_STATE,!GPIO_SIGNAL_ACTIVE_STATE,!GPIO_SIGNAL_ACTIVE_STATE,GPIO_USBERROR_ACTIVE_STATE}; /* Array der zu setzenden Ausgabesignale */

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
		if(e < 0) { /* Wenn kritischer Fehler, gebe Fehlermeldung aus */
			printError("while GPIO initialization!", e);
			iCurExceptionCode = e;
		}
	} /* catch */
/* ENDE DER INITIALISIERUNG */


/* WHILE-LOOP */
	while (iCurExceptionCode == 0) {
/* ERMITTLUNG DES ANGESCHLOSSENEN DEVICES */
		if (inputMovement_curInputDevice == NULL) { /* Wenn kein aktuelles Device vorhanden */
			try {
				inputMovement_curInputDevice = new inputMouse(searchDevicePath("/dev/input/by-path","mouse").c_str());	/* Neues inputMouse-Objekt */
			} catch (int e) {
				if (e < 0) { /* Wenn Fehler beim Erstellen des Maus-Objekts auftritt */
					delete inputMovement_curInputDevice; /* gebe allokierten Speicherplatz wieder frei */
					inputMovement_curInputDevice = NULL; /* und Ruecksetze curInputDevice wieder */
				}
			} /* catch */
		} /* if (inputMovement_curInputDevice == NULL) */

		if (inputMovement_curInputDevice == NULL) { /* Wenn kein aktuelles Device vorhanden */
			try {
				inputMovement_curInputDevice = new inputJoystick(searchDevicePath("/dev/input/by-path","joystick").c_str()); /* Neues inputJoystick-Objekt */
			} catch (int e) {
				if (e < 0) { /* Wenn Fehler beim Erstellen des Joystick-Objekts auftritt */
					delete inputMovement_curInputDevice; /* gebe allokierten Speicherplatz wieder frei */
					inputMovement_curInputDevice = NULL; /* und Ruecksetze curInputDevice wieder */
				}
			} /* catch */
		} /* if (inputMovement_curInputDevice == NULL) */

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
					delete inputMovement_curInputDevice; /* gebe allokierten Speicherplatz wieder frei */
					inputMovement_curInputDevice = NULL; /* setze Zeiger auf aktuelles Device zurueck */
					continue; /* und beginne Schleife von vorne */
				}
				if (e < EXCEPTION_POLLING_ERROR) { /* Im Fehlerfall */
					printError("while reading input device!", e); /* gebe Fehlermeldung aus */
					delete inputMovement_curInputDevice; /* gebe allokierten Speicherplatz wieder frei */
					inputMovement_curInputDevice = NULL; /* setze Zeiger auf aktuelles Device zurueck */
					continue; /* und beginne Schleife von vorne */
				}
			} /* catch */
/* ENDE DER LESE EINGABE */

// TODO Bei Schwellwerten zwischen Maus und Joystick unterscheiden und diese aus config einlesen
/* VERRECHNE WERTE UND MAPPE DIESE AUF AUSGABESIGNALE */
			if(inputMovement_curInputDevice->getDX() > 3) baSignalsToSet[SIGNAL_YF] = GPIO_SIGNAL_ACTIVE_STATE;
			else baSignalsToSet[SIGNAL_YF] = !GPIO_SIGNAL_ACTIVE_STATE;

			if(inputMovement_curInputDevice->getDX() < -3) baSignalsToSet[SIGNAL_YB] = GPIO_SIGNAL_ACTIVE_STATE;
			else baSignalsToSet[SIGNAL_YB] = !GPIO_SIGNAL_ACTIVE_STATE;

			if(inputMovement_curInputDevice->getDY() > 3) baSignalsToSet[SIGNAL_XB] = GPIO_SIGNAL_ACTIVE_STATE;
			else baSignalsToSet[SIGNAL_XB] = !GPIO_SIGNAL_ACTIVE_STATE;

			if(inputMovement_curInputDevice->getDY() < -3) baSignalsToSet[SIGNAL_XF] = GPIO_SIGNAL_ACTIVE_STATE;
			else baSignalsToSet[SIGNAL_XF] = !GPIO_SIGNAL_ACTIVE_STATE;

			if(inputMovement_curInputDevice->getBtn1() == true) baSignalsToSet[SIGNAL_ZF] = GPIO_SIGNAL_ACTIVE_STATE;
			else baSignalsToSet[SIGNAL_ZF] = !GPIO_SIGNAL_ACTIVE_STATE;

			if(inputMovement_curInputDevice->getBtn2() == true) baSignalsToSet[SIGNAL_ZB] = GPIO_SIGNAL_ACTIVE_STATE;
			else baSignalsToSet[SIGNAL_ZB] = !GPIO_SIGNAL_ACTIVE_STATE;
/* ENDE DER VERRECHNE WERTE UND MAPPE DIESE AUF AUSGABESIGNALE */


/* SCHREIBE AUSGABE */
			if (outputGPIOsysfs_RPiGPIO->setSignals(baSignalsToSet) == false) { /* Setze neue Ausgabesignale */
					printError("while setting signals!");
			}

		} /* if (inputMovement_curInputDevice == NULL) ... else */


		try {
			outputGPIOsysfs_RPiGPIO->write(); /* Schreibe Ausgabesignale */
		} catch (int e) {
			if(e < 0) { /* Wenn kritischer Fehler, gebe Fehlermeldung aus */
				printError("while writing GPIO!", e);
				iCurExceptionCode = e;
			}
		} /* catch */
/* ENDE DER SCHREIBE AUSGABE */

	} /* while(iCurExceptionCode == 0) */

	/* Gebe Speicherplatz der Heap-Objekte wieder frei */
	delete inputMovement_curInputDevice;
	delete outputGPIOsysfs_RPiGPIO;
	return iCurExceptionCode;
} /* main() */


