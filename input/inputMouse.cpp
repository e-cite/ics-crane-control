/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: inputMouse.cpp
 * Funktion: Implementierung der Klasse inputMouse, Programmierung der Funktionen
 * Kommentar: Änderung des Einleseverfahrens, Anpassungen dazu in der inputMouse::read()-Funktion, Umbau des Konstruktors, Umstellung auf printf()-Ausgabe
 * Parameter:
 * Rueckgabe:
 * Name: Andreas Dolp
 * Datum: 09.04.2014
 * Version: 0.1
 ---------------------------*/

#include "inputMouse.h"
#include <fcntl.h>			/* open */
#include <unistd.h>			/* read, close */
#include <string.h>			/* strcpy */

using namespace std;

inputMouse::inputMouse(const char* cpMousePathToSet) {
	/* Aufruf des Standardkonstruktors der movementInput-Klasse */
	/* Kopiere cpMousePathToSet in aktuelles Objekt */
	strcpy(this->cpMousePath,cpMousePathToSet);
}


bool inputMouse::read() {
	/* Zeiger auf Maus-Device */
	int pMouseDevice;
	/* Lesebuffer */
	char buffer[SIZE_OF_MOUSE_DATA];
	/* Oeffne Maus-Device-File readonly */
	pMouseDevice = open(this->cpMousePath, O_RDONLY);
	/* Lese Maus-Daten in Lesebuffer */
	int pRead = ::read(pMouseDevice, &buffer, SIZE_OF_MOUSE_DATA);
	/* Wenn Lesen nicht moeglich */
	if (pRead == -1) {
		/* Werfe entsprechende Exception */
		throw EXCEPTION_UNABLE_READ_MOUSE;
	/* Wenn Lesen moeglich */
	} else {
		/* kopiere Inhalt des Lesebuffers in caRawInput, sichere Rohwerte */
		strcpy(this->caRawInput,buffer);
		/* Pruefe auf Linksklick */
		this->bClickLeft = (buffer[0] & 0b00000001) >> 0;
		/* Pruefe auf Rechtsklick */
		this->bClickRight = (buffer[0] & 0b00000010) >> 1;
		/* Pruefe auf Mittelklick */
		this->bClickMiddle = (buffer[0] & 0b00000100) >> 2;
		/* Speichere Wert der relativen X-Verschiebung */
		this->iDX = buffer[1];
		/* Speichere Wert der relativen Y-Verschiebung */
		this->iDY = buffer[2];

		/* Schliesse Datei */
		close(pMouseDevice);
		/* da erfolgreich, gebe true zurueck */
		return true;
	}
	/* Wenn Lesen nicht moeglich, schliesse trotzdem Dateizeiger */
	close(pMouseDevice);
	/* und gebe false zurueck */
	return false;
}

