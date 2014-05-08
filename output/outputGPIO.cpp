/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: outputGPIO.cpp
 * Funktion: Implementierung der Klasse outputGPIO, Programmierung der Funktionen
 * Kommentar: Erstellung einer setActiveUSBErr()-Funktion, um das USB-Fehler-Signal zu setzen und alle anderen Signale zurueckzusetzen
 * Name: Andreas Dolp
 * Datum: 08.05.2014
 * Version: 0.1
 ---------------------------*/

#include "outputGPIO.h"

outputGPIO::outputGPIO() {
	/* Vorbelegung der Elemente mit Initialwerten (Alle Ausgaenge deaktiviert, Fehler aktiv) */
	this->baGPIOSignals[0] = !GPIO_AXES_SIGNALS_ACTIVE;
	this->baGPIOSignals[1] = !GPIO_AXES_SIGNALS_ACTIVE;
	this->baGPIOSignals[2] = !GPIO_AXES_SIGNALS_ACTIVE;
	this->baGPIOSignals[3] = !GPIO_AXES_SIGNALS_ACTIVE;
	this->baGPIOSignals[4] = !GPIO_AXES_SIGNALS_ACTIVE;
	this->baGPIOSignals[5] = !GPIO_AXES_SIGNALS_ACTIVE;
	this->baGPIOSignals[6] = GPIO_USB_ERROR_ACTIVE;
}
/* Setter-Funktion zum Setzen der Signale
 * Prueft die erhaltenen Signale auf Konsistenz und setzt im Fehlerfall USB-Fehler aktiv
 * @param baGPIOSignalsToSet bool-Array der zu schreibenden Ausgangssignale
 * @return TRUE wenn Signale konsistent und zum Schreiben bereit, FALSE wenn nicht
 */
bool outputGPIO::setSignals(const bool baGPIOSignalsToSet[NUM_OF_SIGNALS] = 0) {
	/* Pruefe ob gueltiger Array */
	if( baGPIOSignalsToSet != 0 ) {
// TODO Konsistenzpruefung an im Header definierte Aktiv-Markierungen anpassen
		/* Pruefe auf Konsistenz der Signale: Es duerfen pro Signalpaar nicht beide Signale HIGH sein und USBErr nicht aktiv */
		if ( !(baGPIOSignalsToSet[0] && baGPIOSignalsToSet[1]) && !(baGPIOSignalsToSet[2] && baGPIOSignalsToSet[3]) && !(baGPIOSignalsToSet[4] && baGPIOSignalsToSet[5]) && baGPIOSignalsToSet[6] ) {
			/* Schreibe Werte in eigenes Objekt */
			for(int i = 0; i < NUM_OF_SIGNALS; i++)
				this->baGPIOSignals[i] = baGPIOSignalsToSet[i];

			return true;	/* Da erfolgreiches Setzen, gebe true zurueck */
		} else {
			/* wenn Signalpaare nicht konsistent */
			throw EXCEPTION_INCONSISTENT_SIGNALS_TO_SET;	/* werfe entsprechende Exception */
			this->setActiveUSBErr();	/* und aktiviere USB-Fehler */
		}
	}	/* if( baGPIOSignalsToSet != 0 ) */
	return false;	/* und gebe false zurueck */
}

/*
 * Funktion zum Aktivieren des USB-Errors
 * Dadurch werden automatisch alle anderen Signale auf AUS gesetzt
 */
void outputGPIO::setActiveUSBErr() {
	for(int i = 0; i <= NUM_OF_SIGNALS-2; i++) /* Schleife ueber alle Daten-Signale, nicht jedoch USB-Fehler */
		this->baGPIOSignals[i] = !GPIO_AXES_SIGNALS_ACTIVE; /* Setze Wert jeweils auf nicht aktiv */
	this->baGPIOSignals[NUM_OF_SIGNALS-1] = !GPIO_USB_ERROR_ACTIVE; /* Setze Wert jeweils auf nicht aktiv */
	return;
}


bool outputGPIO::getSignal(const int iNumOfGPIOSignal = -1) {
	/* Pruefe ob zurueckzugebendes Signal im Bereich der zulaessigen Signale */
	if(iNumOfGPIOSignal >= 0 && iNumOfGPIOSignal < NUM_OF_SIGNALS) {
		/* Wenn ja, gebe Wert zurueck */
		return this->baGPIOSignals[iNumOfGPIOSignal];
	}
	/* Wenn nein, werfe entsprechende Exception */
	throw EXCEPTION_NO_VALID_SIGNAL_TO_GET;
	/* und gebe false zurueck */
	return false;
}
