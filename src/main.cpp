/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: main.cpp
 * Funktion: Hauptprojekt
 * Kommentar: Diverse Fehlerverbesserungen
 * Name: Andreas Dolp
 * Datum: 05.06.2014
 * Version: 1.4
 ---------------------------*/

#include "main.h"
#include "readConfig/readConfig.h"
#include "searchDevice/searchDevicePath.h" /* searchDevicePath */
#include "output/outputGPIO.h" /* NUM_OF_SIGNALS, SIGNAL_xx */
#include "input/inputMouse.h" /* new inputMouse */
#include "input/inputMouseDelayed.h" /* new inputMouseDelayed */
#include "input/inputJoystick.h" /* new inputJoystick */
#include "output/outputGPIOsysfs.h" /* outputGPIOsysfs* outputGPIOsysfs_RPiGPIO = new outputGPIOsysfs */
#ifdef DEBUG
#ifdef NCURSES
#define GLOBAL_WINDOW /* UNBEDINGT ERFORDERLICH FUER GLOBALE NCURSES-VARIABLEN */
#endif /* NCURSES */
#include "print/print.h" /* print() */
#include <thread> /* thread */
#endif /* DEBUG */
#include <unistd.h> /* sleep, usleep */

/*
 * @brief Hauptfunktion zur Steuerung aller beteiligten Programmteile
 * @return 0 bei korrekter Ausfuehrung, Exception-Code der Unterfunktionen (negativ) bei Abbruch durch kritischen Fehler
 */
int main ( int argc, char* argv[] ) {
/* DEKLARATION UND DEFINITION */
	int iCurExceptionCode = 0; /* Enthaelt aktuellen ExceptionCode */
	struct {
		int iX;
		int iY;
	} structCurThresholdValues = {DEFAULT_DELTA_MIN,DEFAULT_DELTA_MIN}; /* Neue struct, beinhaltet aktuell gueltige Schwellwerte der X- und Y-Bewegungen */
	unsigned int iaGPIOAddresses[NUM_OF_SIGNALS] = {7,17,27,22,10,9,11}; /* Array der GPIO-Ausgabepins, Reihenfolge USBErr,XF,XB,YF,YB,ZF,ZB; siehe outputGPIO.h */
	configValues* configValuespConfigData = new (configValues); /* struct configValues, enthaelt die eingelesenen Konfigurationsdaten */
	bool baSignalsToSet[NUM_OF_SIGNALS] = {!GPIO_SIGNAL_ACTIVE_STATE,!GPIO_SIGNAL_ACTIVE_STATE,!GPIO_SIGNAL_ACTIVE_STATE,!GPIO_SIGNAL_ACTIVE_STATE,!GPIO_SIGNAL_ACTIVE_STATE,!GPIO_SIGNAL_ACTIVE_STATE,GPIO_USBERROR_ACTIVE_STATE}; /* Array der zu setzenden Ausgabesignale */

	inputMovement* inputMovement_curInputDevice = 0; /* Polymorpher Zeiger auf inputMovement-Objekt, gibt aktuell gueltiges Objekt an */
	outputGPIOsysfs* outputGPIOsysfs_RPiGPIO = new outputGPIOsysfs(iaGPIOAddresses); /* Neues outputGPIOsysfs-Objekt */
/* ENDE DER DEKLARATION UND DEFINITION */

/* INITIALISIERUNG */
#ifdef DEBUG
	printInit(); /* Initialisiere ncurses-Windows */
	printTitle(); /* Gebe Titel aus */
	std::thread threadPrintSignals (printInit_SignalsThread,outputGPIOsysfs_RPiGPIO); /* Print-Funktion in eigenem Thread */
#endif /* DEBUG */

	if (readConfig(configValuespConfigData,"../config.ini") == 0) { /* Lese Werte aus Config-Datei und schreibe GPIO-Adressen */
		iaGPIOAddresses[SIGNAL_USBERR] = configValuespConfigData->iGpioUSBError;
		iaGPIOAddresses[SIGNAL_XF] = configValuespConfigData->iGpioXF;
		iaGPIOAddresses[SIGNAL_XB] = configValuespConfigData->iGpioXB;
		iaGPIOAddresses[SIGNAL_YF] = configValuespConfigData->iGpioYF;
		iaGPIOAddresses[SIGNAL_YB] = configValuespConfigData->iGpioYB;
		iaGPIOAddresses[SIGNAL_ZF] = configValuespConfigData->iGpioZF;
		iaGPIOAddresses[SIGNAL_ZB] = configValuespConfigData->iGpioZB;
	} else { /* wenn keine gueltige Konfiguration gelesen */
		configValuespConfigData->iDeltaRelXMin = DEFAULT_DELTA_MIN; /* Belege mit Standard-Werten */
		configValuespConfigData->iDeltaRelYMin = DEFAULT_DELTA_MIN; /* iGpio... war bereits bei Initialisierung mit Standard-Werten belegt */
		configValuespConfigData->iDeltaAbsXMin = DEFAULT_DELTA_MIN;
		configValuespConfigData->iDeltaAbsYMin = DEFAULT_DELTA_MIN;
		configValuespConfigData->iDelaySeconds = DEFAULT_DELAY_SECONDS;
		configValuespConfigData->iDelayMicroseconds = DEFAULT_DELAY_MICROSECONDS;
#ifdef DEBUG
		printError("reading Config-File, using default config-values"); /* gebe Warnung aus */
#endif /* DEBUG */
	} /* if (readConfig(configValuespConfigData,"config.ini") == 0) ... else */

	try {
		outputGPIOsysfs_RPiGPIO->init(); /* Initialisiere GPIO-Ausgaenge */
	} catch (int e) {
		if(e < 0) { /* Wenn kritischer Fehler */
#ifdef DEBUG
			printError("while GPIO initialization!", e); /* gebe Fehlermeldung aus */
#endif /* DEBUG */
			iCurExceptionCode = e;
		}
	} /* catch */
/* ENDE DER INITIALISIERUNG */


/* WHILE-LOOP */
	while (iCurExceptionCode == 0) {
/* ERMITTLUNG DES ANGESCHLOSSENEN DEVICES */
/* PROBIERE inputMouseDelayed */
		if (inputMovement_curInputDevice == 0) { /* Wenn kein aktuelles Device vorhanden */
			try {
				inputMovement_curInputDevice = new inputMouseDelayed(searchDevicePath("/dev/input/by-path","mouse").c_str());	/* Neues inputMouseDelay-Objekt */
				structCurThresholdValues.iX = configValuespConfigData->iDeltaRelXMin; /* setze aktuelle Schwellwerte */
				structCurThresholdValues.iY = configValuespConfigData->iDeltaRelYMin;
				((inputMouseDelayed*)inputMovement_curInputDevice)->setTimevalKlickDelay(configValuespConfigData->iDelaySeconds,configValuespConfigData->iDelayMicroseconds); /* Schreibe aktuelle Verzoegerungswerte in Objekt */
			} catch (int e) {
				if (e < 0) { /* Wenn Fehler beim Erstellen des Maus-Objekts auftritt */
					delete inputMovement_curInputDevice; /* gebe allokierten Speicherplatz wieder frei */
					inputMovement_curInputDevice = 0; /* und Ruecksetze curInputDevice wieder */
				}
			} /* catch */
		} /* if (inputMovement_curInputDevice == 0) */

/* PROBIERE inputMouse */
		if (inputMovement_curInputDevice == 0) { /* Wenn kein aktuelles Device vorhanden */
			try {
				inputMovement_curInputDevice = new inputMouse(searchDevicePath("/dev/input/by-path","mouse").c_str());	/* Neues inputMouse-Objekt */
				structCurThresholdValues.iX = configValuespConfigData->iDeltaRelXMin; /* setze aktuelle Schwellwerte */
				structCurThresholdValues.iY = configValuespConfigData->iDeltaRelYMin;
			} catch (int e) {
				if (e < 0) { /* Wenn Fehler beim Erstellen des Maus-Objekts auftritt */
					delete inputMovement_curInputDevice; /* gebe allokierten Speicherplatz wieder frei */
					inputMovement_curInputDevice = 0; /* und Ruecksetze curInputDevice wieder */
				}
			} /* catch */
		} /* if (inputMovement_curInputDevice == 0) */

/* PROBIERE inputJoystick */
		if (inputMovement_curInputDevice == 0) { /* Wenn kein aktuelles Device vorhanden */
			try {
				inputMovement_curInputDevice = new inputJoystick(searchDevicePath("/dev/input/by-path","joystick").c_str()); /* Neues inputJoystick-Objekt */
				structCurThresholdValues.iX = configValuespConfigData->iDeltaAbsXMin; /* setze aktuelle Schwellwerte */
				structCurThresholdValues.iY = configValuespConfigData->iDeltaAbsYMin;
				((inputJoystick*)inputMovement_curInputDevice)->calculateThreshold(&structCurThresholdValues.iX,&structCurThresholdValues.iY); /* Berechne aktuelle Schwellwerte und setze diese */

			} catch (int e) {
				if (e < 0) { /* Wenn Fehler beim Erstellen des Joystick-Objekts auftritt */
					delete inputMovement_curInputDevice; /* gebe allokierten Speicherplatz wieder frei */
					inputMovement_curInputDevice = 0; /* und Ruecksetze curInputDevice wieder */
				}
			} /* catch */
		} /* if (inputMovement_curInputDevice == 0) */

/* KEIN DEVICE ERKANNT */
		if (inputMovement_curInputDevice == 0) { /* Wenn kein Device gefunden */
			outputGPIOsysfs_RPiGPIO->setUSBErrActive(); /* setze USB-Fehler, ruecksetze alle Signale */
#ifdef DEBUG
			printError("no input device found"); /* gebe Warnung aus */
#endif /* DEBUG */
			structCurThresholdValues.iX = configValuespConfigData->iDeltaRelXMin; /* und setze Default-Werte*/
			structCurThresholdValues.iY = configValuespConfigData->iDeltaRelYMin;
#ifdef DEBUG
			sleep(2); /* sleep um hohe CPU-Last zu vermeiden */
#else /* DEBUG */
			usleep(1000);
#endif /* DEBUG */
		} /* if (inputMovement_curInputDevice == 0) */

/* ENDE DER ERMITTLUNG DES ANGESCHLOSSENEN DEVICES */
		else {

/* AB HIER GEULTIGES DEVICE ERMITTELT */
			baSignalsToSet[SIGNAL_USBERR] = !GPIO_USBERROR_ACTIVE_STATE; /* Da gueltiges Device, Ruecksetze USB-Fehler */

/* LESE EINGABE */
			try {
				inputMovement_curInputDevice->read();
			} catch (int e) {
				if (e == EXCEPTION_POLLING_ERROR) { /* Im Fehlerfall */
#ifdef DEBUG
					printError("while polling input device!", e); /* gebe Fehlermeldung aus */
#endif /* DEBUG */
					delete inputMovement_curInputDevice; /* gebe allokierten Speicherplatz wieder frei */
					inputMovement_curInputDevice = 0; /* setze Zeiger auf aktuelles Device zurueck */
					outputGPIOsysfs_RPiGPIO->setUSBErrActive(); /* setze USB-Fehler, ruecksetze alle Signale */
					goto WRITE_OUTPUT; /* und springe WRITE_OUTPUT (Schreiben der Ausgaenge) an */
				}
				if (e < EXCEPTION_POLLING_ERROR) { /* Im Fehlerfall */
#ifdef DEBUG
					printError("while reading input device!", e); /* gebe Fehlermeldung aus */
#endif /* DEBUG */
					delete inputMovement_curInputDevice; /* gebe allokierten Speicherplatz wieder frei */
					inputMovement_curInputDevice = 0; /* setze Zeiger auf aktuelles Device zurueck */
					outputGPIOsysfs_RPiGPIO->setUSBErrActive(); /* setze USB-Fehler, ruecksetze alle Signale */
					goto WRITE_OUTPUT; /* und springe WRITE_OUTPUT (Schreiben der Ausgaenge) an */
				}
			} /* catch */
/* ENDE DER LESE EINGABE */

/* VERRECHNE WERTE UND MAPPE DIESE AUF AUSGABESIGNALE */
			if(inputMovement_curInputDevice->getDX() > structCurThresholdValues.iY) baSignalsToSet[SIGNAL_YF] = GPIO_SIGNAL_ACTIVE_STATE;
			else baSignalsToSet[SIGNAL_YF] = !GPIO_SIGNAL_ACTIVE_STATE;

			if(inputMovement_curInputDevice->getDX() < -structCurThresholdValues.iY) baSignalsToSet[SIGNAL_YB] = GPIO_SIGNAL_ACTIVE_STATE;
			else baSignalsToSet[SIGNAL_YB] = !GPIO_SIGNAL_ACTIVE_STATE;

			if(inputMovement_curInputDevice->getDY() > structCurThresholdValues.iX) baSignalsToSet[SIGNAL_XB] = GPIO_SIGNAL_ACTIVE_STATE;
			else baSignalsToSet[SIGNAL_XB] = !GPIO_SIGNAL_ACTIVE_STATE;

			if(inputMovement_curInputDevice->getDY() < -structCurThresholdValues.iX) baSignalsToSet[SIGNAL_XF] = GPIO_SIGNAL_ACTIVE_STATE;
			else baSignalsToSet[SIGNAL_XF] = !GPIO_SIGNAL_ACTIVE_STATE;

			if(inputMovement_curInputDevice->getBtn1() == true) baSignalsToSet[SIGNAL_ZF] = GPIO_SIGNAL_ACTIVE_STATE;
			else baSignalsToSet[SIGNAL_ZF] = !GPIO_SIGNAL_ACTIVE_STATE;

			if(inputMovement_curInputDevice->getBtn2() == true) baSignalsToSet[SIGNAL_ZB] = GPIO_SIGNAL_ACTIVE_STATE;
			else baSignalsToSet[SIGNAL_ZB] = !GPIO_SIGNAL_ACTIVE_STATE;
/* ENDE DER VERRECHNE WERTE UND MAPPE DIESE AUF AUSGABESIGNALE */


/* SCHREIBE AUSGABE */
			if (outputGPIOsysfs_RPiGPIO->setSignals(baSignalsToSet) == false) { /* Setze neue Ausgabesignale */
#ifdef DEBUG
					printError("while setting signals!");
#else
					; /* Noetig, da sonst compiler-Fehler, wenn DEBUG nicht gesetzt */
#endif /* DEBUG */
			}

		} /* if (inputMovement_curInputDevice == 0) ... else */

WRITE_OUTPUT: /* Sprungmarke: Schreiben der Ausgaenge, wird NUR im Fehlerfall angesprungen */
		try {
			outputGPIOsysfs_RPiGPIO->write(); /* Schreibe Ausgabesignale */
		} catch (int e) {
			if(e < 0) { /* Wenn kritischer Fehler, gebe Fehlermeldung aus */
#ifdef DEBUG
				printError("while writing GPIO!", e);
#endif /* DEBUG */
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
