/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: outputGPIO.cpp
 * Funktion: Implementierung der Klasse outputGPIO, Programmierung der Methoden
 * Kommentar: Ueberarbeitungen, erste vollstaendig lauffaehige Version
 * Name: Andreas Dolp
 * Datum: 08.05.2014
 * Version: 1.0
 ---------------------------*/

#include "outputGPIO.h"

/*
 * Konstruktor
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
 * Setter-Methode zum Setzen der Signale
 * Prueft die erhaltenen Signale auf Konsistenz und ruft im Fehlerfall setUSBErrActive() auf
 * @param baGPIOSignalsToSet bool-Array der zu schreibenden Ausgangssignale
 * @return TRUE wenn Signale konsistent und zum Schreiben bereit, FALSE wenn nicht
 * @except EXCEPTION_INCONSISTENT_SIGNALS_TO_SET Fehler beim Setzen der Signale, da inkonsistente Signale uebergeben wurden
 */
bool outputGPIO::setSignals(const bool baGPIOSignalsToSet[NUM_OF_SIGNALS]) {
	/* Konsistenzpruefung der Signale: Es duerfen pro Signalpaar nicht beide Signale AKTIV sein sowie USBErr nicht AKTIV */
	if ( !((baGPIOSignalsToSet[SIGNAL_XF] == GPIO_SIGNAL_ACTIVE_STATE) && (baGPIOSignalsToSet[SIGNAL_XB] == GPIO_SIGNAL_ACTIVE_STATE)) ) {
		if ( !((baGPIOSignalsToSet[SIGNAL_YF] == GPIO_SIGNAL_ACTIVE_STATE) && (baGPIOSignalsToSet[SIGNAL_YB] == GPIO_SIGNAL_ACTIVE_STATE)) ) {
			if ( !((baGPIOSignalsToSet[SIGNAL_ZF] == GPIO_SIGNAL_ACTIVE_STATE) && (baGPIOSignalsToSet[SIGNAL_ZB] == GPIO_SIGNAL_ACTIVE_STATE)) ) {
				if ( baGPIOSignalsToSet[SIGNAL_USBERR] != GPIO_USBERROR_ACTIVE_STATE ) {

					for(int i = 0; i < NUM_OF_SIGNALS; i++) /* Schleife ueber alle Signale */
						this->baGPIOSignals[i] = baGPIOSignalsToSet[i]; /* Schreibe Wert in eigenes Objekt */

					return true; /* Da erfolgreiches Setzen, gebe TRUE zurueck */
				}
			}
		}
	}

	/* wenn Signalpaare nicht konsistent oder USBErr AKTIV */
	throw EXCEPTION_INCONSISTENT_SIGNALS_TO_SET; /* werfe entsprechende Exception */
	this->setUSBErrActive(); /* setze USB-Fehler AKTIV */
	return false; /* und gebe FALSE zurueck */
}

/*
 * Setter-Methode zum AKTIV Setzen des USB-Errors
 * dadurch werden alle uebrigen Signale NICHT AKTIV gesetzt
 */
void outputGPIO::setUSBErrActive() {
	for(int i = SIGNAL_USBERR+1; i < NUM_OF_SIGNALS; i++) /* Schleife ueber alle Signale, nicht jedoch USB-Fehler */
		this->baGPIOSignals[i] = !GPIO_SIGNAL_ACTIVE_STATE; /* Setze Signal jeweils auf NICHT AKTIV */
	this->baGPIOSignals[SIGNAL_USBERR] = GPIO_USBERROR_ACTIVE_STATE; /* Setze USBErr auf AKTIV */
	return;
}

/*
 * Getter-Methode liefert den Wert des im Parameter uebergebenen Signals
 * @param iGPIOSignalToGet Signal-Code / -Nummer des abzufragenden Signals
 * @return Signalzustand des angefragten Signals
 */
bool outputGPIO::getSignal(const unsigned int iGPIOSignalToGet) {
	if((iGPIOSignalToGet >= 0) && (iGPIOSignalToGet < NUM_OF_SIGNALS)) /* Pruefe ob angefragtes Signal im Bereich der zulaessigen Signale */
		return this->baGPIOSignals[iGPIOSignalToGet]; /* Wenn ja, gebe dessen Wert zurueck */

	throw EXCEPTION_PARAMETER_OUT_OF_RANGE; /* Wenn nein, werfe entsprechende Exception */
	return false; /* und gebe FALSE zurueck */
}
