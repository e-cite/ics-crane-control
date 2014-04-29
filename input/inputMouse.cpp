/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: inputMouse.cpp
 * Funktion: Implementierung der Klasse inputMouse, Programmierung der Funktionen
 * Kommentar: Problembehebung der verlorenen input-events durch Oeffnen des Dateizeigers im Konstruktor, Einbau des Destruktors (Dateizeiger schliessen)
 * Name: Andreas Dolp
 * Datum: 29.04.2014
 * Version: 0.4
 ---------------------------*/

#include "inputMouse.h"
#include <fcntl.h>			/* open */
#include <unistd.h>			/* read, close */
#include <string.h>			/* strcpy */
#include <linux/input.h>	/* struct input_event */
#include <poll.h>			/* struct pollfd, poll */

using namespace std;

inputMouse::inputMouse(const char* cpMousePathToSet) {
	/* Aufruf des Standardkonstruktors der movementInput-Klasse */
	/* Kopiere cpMousePathToSet in aktuelles Objekt */
	strcpy(this->cpMousePath,cpMousePathToSet);
	fds.events = POLLIN;	/* Initialisiere fds.events mit POLLIN Bitmaske. POLLIN = pruefe ob einkommende Daten vorhanden */
	/* Oeffne Maus-Device-File readonly */
	/* Wenn Oeffnen nicht moeglich */
	if ((fds.fd = open(this->cpMousePath, O_RDONLY)) == -1)
		/* Werfe entsprechende Exception */
		throw EXCEPTION_UNABLE_READ_MOUSE;
}

inputMouse::~inputMouse() {
	/* Schliesse Datei */
	close(fds.fd);
}


bool inputMouse::read() {
	/* Variable zum Speichern des Rueckgabewerts der poll()-Funktion */
	int iPollRetVal = -1;

	/* input_event struct des aktuellen Lesevorgangs */
	struct input_event ie;

	/* Wenn Datei in Konstruktor korrekt geoeffnet */
	if (fds.fd != -1) {
		/* Aufruf der poll()-Funktion mit Adresse der pollfd-struct, lese 1 struct und Timeout in ms */
		iPollRetVal = poll(&fds,1,POLLING_TIMEOUT_MS);
		/* Wenn polling erfolgreich, d.h. Daten anstehend */
		if (iPollRetVal > 0) {
			/* Lese Inhalt des file-descriptors in die input_event struct ie */
			::read(fds.fd, &ie, sizeof(struct input_event));
			/* Da beim Einlesen von Klicks zwei Events auftreten, lese bei ie.type = EV_MSC(=4) nochmals */
			if (ie.type == EV_MSC)
				::read(fds.fd, &ie, sizeof(struct input_event));
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
		} /* if (iPollRetVal > 0) (polling erfolgreich, d.h. Daten anstehend) */

		/* Wenn poll() mit Timeout beendet wird */
		if (iPollRetVal == 0) {
			/* Setze X-Verschiebung = 0 */
			this->iDX = 0;
			/* Setze X-Verschiebung = 0 */
			this->iDY = 0;
			/* werfe entsprechende Exception */
			throw EXCEPTION_POLLING_TIMEOUT;
		} /* if (iPollRetVal == 0) */

		/* Wenn poll() mit Error beendet wird */
		if (iPollRetVal < 0) {
			/* werfe entsprechende Exception */
			throw EXCEPTION_POLLING_ERROR;
			/* und gebe false zurueck */
			return false;
		}

		/* da erfolgreich, gebe true zurueck */
		return true;

	} /* if Device korrekt geoeffnet */

	/* Wenn Lesen nicht moeglich gebe false zurueck */
	return false;
}

