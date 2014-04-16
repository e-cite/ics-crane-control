/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: inputMouse.cpp
 * Funktion: Implementierung der Klasse inputMouse, Programmierung der Funktionen
 * Kommentar: Umbau auf input_event Struktur, da bessere Weiterverwendbarkeit, insbesondere mit Joystick
 * Name: Andreas Dolp
 * Datum: 16.04.2014
 * Version: 0.2
 ---------------------------*/

#include "inputMouse.h"
#include <fcntl.h>			/* open */
#include <unistd.h>			/* read, close */
#include <string.h>			/* strcpy */
#include <linux/input.h>	/* struct input_event */

using namespace std;

inputMouse::inputMouse(const char* cpMousePathToSet) {
	/* Aufruf des Standardkonstruktors der movementInput-Klasse */
	/* Kopiere cpMousePathToSet in aktuelles Objekt */
	strcpy(this->cpMousePath,cpMousePathToSet);
}


bool inputMouse::read() {
	/* Zeiger auf Maus-Device */
	int fd;

	/* input_event struct des aktuellen Lesevorgangs */
	struct input_event ie;

	/* Oeffne Maus-Device-File readonly */
	/* Wenn Lesen nicht moeglich */
	if ((fd = open(this->cpMousePath, O_RDONLY)) == -1) {
		/* Werfe entsprechende Exception */
		throw EXCEPTION_UNABLE_READ_MOUSE;
	/* Wenn Lesen moeglich */
	} else {
		/* Lese solange, bis geeigneter return-Wert von ::read auftritt */
		while(ie.type != EV_KEY && ie.type != EV_REL)
			// Lese Inhalt des file-descriptors in die input_event struct
			::read(fd, &ie, sizeof(struct input_event));

		/* Pruefe auf Linksklick */
		if (ie.type == EV_KEY && ie.code == BTN_LEFT)
			this->bClickLeft = ie.value;

		/* Pruefe auf Rechtsklick */
		if (ie.type == EV_KEY && ie.code == BTN_RIGHT)
			this->bClickRight = ie.value;

		/* Pruefe auf Mittelklick */
		if (ie.type == EV_KEY && ie.code == BTN_MIDDLE)
			this->bClickMiddle = ie.value;

		/* Pruefe auf X-Verschiebung */
		if (ie.type == EV_REL && ie.code == REL_X)
			this->iDX = ie.value;

		/* Pruefe auf Y-Verschiebung */
		if (ie.type == EV_REL && ie.code == REL_Y)
			this->iDY = ie.value;

		/* Schliesse Datei */
		close(fd);
		/* da erfolgreich, gebe true zurueck */
		return true;
	}
	/* Wenn Lesen nicht moeglich, schliesse trotzdem Dateizeiger */
	close(fd);
	/* und gebe false zurueck */
	return false;
}

