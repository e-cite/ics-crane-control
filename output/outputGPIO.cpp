/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: outputGPIO.cpp
 * Funktion: Implementierung der Klasse outputGPIO, Programmierung der Funktionen
 * Kommentar: Erste Version
 * Name: Andreas Dolp
 * Datum: 22.04.2014
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

bool outputGPIO::setSignals(bool baGPIOSignalsToSet[NUM_OF_SIGNALS] = 0) {
	/* Pruefe ob gueltiger Array */
	if( baGPIOSignalsToSet != 0 ) {
		/* Pruefe auf Konsistenz der Signale: Es duerfen pro Signalpaar nicht beide Signale HIGH sein */
		if ( !(baGPIOSignalsToSet[0] && baGPIOSignalsToSet[1]) && !(baGPIOSignalsToSet[2] && baGPIOSignalsToSet[3]) && !(baGPIOSignalsToSet[4] && baGPIOSignalsToSet[5]) ) {
			/* Schreibe Werte in eigenes Objekt */
			for(int i = 0; i < NUM_OF_SIGNALS; i++)
				this->baGPIOSignals[i] = baGPIOSignalsToSet[i];

			return true;	/* Da erfolgreiches Setzen, gebe true zurueck */
		} else {
			/* wenn Signalpaare nicht konsistent, werfe entsprechende Exception */
			throw EXCEPTION_INCONSISTENT_SIGNALS_TO_SET;
		}
	}	/* if( baGPIOSignalsToSet != 0 ) */
	return false;	/* und gebe false zurueck */
}

bool outputGPIO::getSignal(int iNumOfGPIOSignal = -1) {
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
