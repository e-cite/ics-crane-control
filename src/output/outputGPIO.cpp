/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: outputGPIO.cpp
 * Funktion: Implementierung der Klasse outputGPIO, Programmierung der Methoden
 * Kommentar: Fehlerverbesserungen, Anpassung an Signalkonvention, bei beide Signale AKTIV werden beide auf NICHT AKTIV gesetzt
 * Name: Andreas Dolp
 * Datum: 16.05.2014
 * Version: 1.1
 ---------------------------*/

#include "outputGPIO.h"

/*
 * @brief Konstruktor
 *
 * Vorbelegung der Elemente mit Initialwerten (Alle Signale NICHT AKTIV, USB-Fehler AKTIV)
 */
outputGPIO::outputGPIO() {
	this->baGPIOSignals[SIGNAL_USBERR] = GPIO_USBERROR_ACTIVE_STATE;
	this->baGPIOSignals[SIGNAL_XF] = !GPIO_SIGNAL_ACTIVE_STATE;
	this->baGPIOSignals[SIGNAL_XB] = !GPIO_SIGNAL_ACTIVE_STATE;
	this->baGPIOSignals[SIGNAL_YF] = !GPIO_SIGNAL_ACTIVE_STATE;
	this->baGPIOSignals[SIGNAL_YB] = !GPIO_SIGNAL_ACTIVE_STATE;
	this->baGPIOSignals[SIGNAL_ZF] = !GPIO_SIGNAL_ACTIVE_STATE;
	this->baGPIOSignals[SIGNAL_ZB] = !GPIO_SIGNAL_ACTIVE_STATE;
}

/*
 * @brief Setter-Methode zum Setzen der Signale
 * @param baGPIOSignalsToSet bool-Array der zu schreibenden Ausgangssignale
 * @return TRUE wenn Signale erfolgreich gesetzt und zum Schreiben bereit, FALSE wenn nicht
 *
 * Prueft die erhaltenen Signale und setzt im Zweifelsfall auf NICHT AKTIV
 */
bool outputGPIO::setSignals(const bool baGPIOSignalsToSet[NUM_OF_SIGNALS]) {
	if ( baGPIOSignalsToSet[SIGNAL_USBERR] != GPIO_USBERROR_ACTIVE_STATE ) { /* Pruefe ob USB-Fehler aktiv */
		for(int i = 0; i < NUM_OF_SIGNALS; i++) /* Schleife ueber alle Signale */
			this->baGPIOSignals[i] = baGPIOSignalsToSet[i]; /* Schreibe Wert in eigenes Objekt */
	} else /* Wenn USB-Fehler aktiv */
		return false;


	/* Wenn beide Signale aktiv, setze beide zugehoerige Ausgaenge auf NICHT AKTIV*/
	/* SIGNAL_X... */
	if ( (baGPIOSignalsToSet[SIGNAL_XF] == GPIO_SIGNAL_ACTIVE_STATE) && (baGPIOSignalsToSet[SIGNAL_XB] == GPIO_SIGNAL_ACTIVE_STATE) ) {
		this->baGPIOSignals[SIGNAL_XF] = !GPIO_SIGNAL_ACTIVE_STATE;
		this->baGPIOSignals[SIGNAL_XB] = !GPIO_SIGNAL_ACTIVE_STATE;
	}
	/* SIGNAL_Y... */
	if ( (baGPIOSignalsToSet[SIGNAL_YF] == GPIO_SIGNAL_ACTIVE_STATE) && (baGPIOSignalsToSet[SIGNAL_YB] == GPIO_SIGNAL_ACTIVE_STATE) ) {
		this->baGPIOSignals[SIGNAL_YF] = !GPIO_SIGNAL_ACTIVE_STATE;
		this->baGPIOSignals[SIGNAL_YB] = !GPIO_SIGNAL_ACTIVE_STATE;
	}
	/* SIGNAL_Z... */
	if ( (baGPIOSignalsToSet[SIGNAL_ZF] == GPIO_SIGNAL_ACTIVE_STATE) && (baGPIOSignalsToSet[SIGNAL_ZB] == GPIO_SIGNAL_ACTIVE_STATE) ) {
		this->baGPIOSignals[SIGNAL_ZF] = !GPIO_SIGNAL_ACTIVE_STATE;
		this->baGPIOSignals[SIGNAL_ZB] = !GPIO_SIGNAL_ACTIVE_STATE;
	}

	return true; /* Da erfolgreich, gebe TRUE zurueck */
}

/*
 * @brief Setter-Methode zum AKTIV Setzen des USB-Errors
 *
 * dadurch werden alle uebrigen Signale NICHT AKTIV gesetzt
 */
void outputGPIO::setUSBErrActive() {
	for(int i = SIGNAL_USBERR+1; i < NUM_OF_SIGNALS; i++) /* Schleife ueber alle Signale, nicht jedoch USB-Fehler */
		this->baGPIOSignals[i] = !GPIO_SIGNAL_ACTIVE_STATE; /* Setze Signal jeweils auf NICHT AKTIV */
	this->baGPIOSignals[SIGNAL_USBERR] = GPIO_USBERROR_ACTIVE_STATE; /* Setze USBErr auf AKTIV */
	return;
}

/*
 * @brief Getter-Methode liefert den Wert des im Parameter uebergebenen Signals
 * @param iGPIOSignalToGet Signal-Code / -Nummer des abzufragenden Signals
 * @return Signalzustand des angefragten Signals
 */
bool outputGPIO::getSignal(const unsigned int iGPIOSignalToGet) {
	if((iGPIOSignalToGet >= 0) && (iGPIOSignalToGet < NUM_OF_SIGNALS)) /* Pruefe ob angefragtes Signal im Bereich der zulaessigen Signale */
		return this->baGPIOSignals[iGPIOSignalToGet]; /* Wenn ja, gebe dessen Wert zurueck */

	throw EXCEPTION_PARAMETER_OUT_OF_RANGE; /* Wenn nein, werfe entsprechende Exception */
	return false; /* und gebe FALSE zurueck */
}
