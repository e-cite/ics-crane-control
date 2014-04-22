/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: inputMouse.cpp
 * Funktion: Implementierung der Klasse inputMouse, Programmierung der Funktionen
 * Kommentar: Aufgrund von Ruecksetzproblem Umbau auf polling-Funktionalitaet
 * Name: Andreas Dolp
 * Datum: 17.04.2014
 * Version: 0.3
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
}


bool inputMouse::read() {
	/* struct fuer file-descriptor, zu ueberwachende poll-events */
	struct pollfd fds;
	/* Initialisiere fds.events mit POLLIN Bitmaske. POLLIN = pruefe ob einkommende Daten vorhanden */
	fds.events = POLLIN;
	/* Variable zum Speichern des Rueckgabewerts der poll()-Funktion */
	int iPollRetVal = -1;

	/* input_event struct des aktuellen Lesevorgangs */
	struct input_event ie;

	/* Oeffne Maus-Device-File readonly */
	/* Wenn Lesen nicht moeglich */
	if ((fds.fd = open(this->cpMousePath, O_RDONLY)) == -1) {
		/* Werfe entsprechende Exception */
		throw EXCEPTION_UNABLE_READ_MOUSE;
	/* Wenn Lesen moeglich */
	} else {
		/* Polling-Mode fuer Device-Pointer aktivieren */
		int flags = fcntl(fds.fd, F_GETFL, 0);
		fcntl(fds.fd, F_SETFL, flags | O_NONBLOCK);
		/* Aufruf der poll()-Funktion mit Adresse der pollfd-struct, lese 1 struct und Timeout in ms */
		iPollRetVal = poll(&fds,1,POLLING_TIMEOUT_MS);
		/* Wenn polling erfolgreich, d.h. Daten anstehend */
		if (iPollRetVal > 0) {
			/* Lese Inhalt des file-descriptors in die input_event struct */
			::read(fds.fd, &ie, sizeof(struct input_event));
			/* Da beim Einlesen von Klicks zwei Events auftreten, lese bei ie.type = 4 nochmals */
			if (ie.type == EV_MSC) {
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
			} else {
				/* Pruefe auf X-Verschiebung */
				if (ie.type == EV_REL && ie.code == REL_X)
					this->iDX = ie.value;
					/* Pruefe auf Y-Verschiebung */
				if (ie.type == EV_REL && ie.code == REL_Y)
					this->iDY = ie.value;
			} /* if (ie.type == EV_MSC) ... else */

		} /* if (iPollRetVal > 0) */

		/* Schliesse Datei */
		close(fds.fd);

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

	} /* else if (Device korrekt geoeffnet)*/

	/* Wenn Lesen nicht moeglich gebe false zurueck */
	return false;
}

