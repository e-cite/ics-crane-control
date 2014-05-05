/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: inputJoystick.cpp
 * Funktion: Implementierung der Klasse inputJoystick, Programmierung der Funktionen
 * Kommentar: Erste Version, basierend auf inputMouse 0.4
 * Name: Andreas Dolp
 * Datum: 06.05.2014
 * Version: 0.1
 ---------------------------*/

#include "inputJoystick.h"
#include <fcntl.h>			/* open */
#include <unistd.h>			/* read, close */
#include <string.h>			/* strcpy */
#include <linux/input.h>	/* struct input_event */
#include <poll.h>			/* struct pollfd, poll */

using namespace std;

inputJoystick::inputJoystick(const char* cpJoystickPathToSet) {
	/* Aufruf des Standardkonstruktors der movementInput-Klasse */
	/* Kopiere cpJoystickPathToSet in aktuelles Objekt */
	strcpy(this->cpJoystickPath,cpJoystickPathToSet);
	fds.events = POLLIN;	/* Initialisiere fds.events mit POLLIN Bitmaske. POLLIN = pruefe ob einkommende Daten vorhanden */
	/* Oeffne Maus-Device-File readonly */
	/* Wenn Oeffnen nicht moeglich */
	if ((fds.fd = open(this->cpJoystickPath, O_RDONLY)) == -1)
		/* Werfe entsprechende Exception */
		throw EXCEPTION_UNABLE_READ_JOYSTICK;
}

inputJoystick::~inputJoystick() {
	/* Schliesse Datei */
	close(fds.fd);
}


bool inputJoystick::read() {
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
			if (ie.type == EV_KEY && ie.code == BTN_TRIGGER)
				this->bClickLeft = ie.value;
			/* Pruefe auf Rechtsklick */
			if (ie.type == EV_KEY && ie.code == BTN_THUMB)
				this->bClickRight = ie.value;
			/* Pruefe auf Mittelklick */
			if (ie.type == EV_KEY && ie.code == BTN_THUMB2)
				this->bClickMiddle = ie.value;
			/* Pruefe auf X-Verschiebung */
			if (ie.type == EV_ABS && ie.code == ABS_X)
				this->iDX = ie.value;
			/* Pruefe auf Y-Verschiebung */
			if (ie.type == EV_ABS && ie.code == ABS_Y)
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
