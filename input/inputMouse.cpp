/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: inputMouse.cpp
 * Funktion: Implementierung der Klasse inputMouse, Programmierung der Methoden
 * Kommentar: Ueberarbeitungen, erste vollstaendig lauffaehige Version, Einbau Fehlerauswertung ::read-Methode
 * Name: Andreas Dolp
 * Datum: 16.05.2014
 * Version: 1.1
 ---------------------------*/

#include "inputMouse.h"
#include <fcntl.h> /* open */
#include <unistd.h> /* read, close */
#include <linux/input.h> /* struct input_event */
#include <poll.h> /* struct pollfd, poll */

/*
 * @brief Konstruktor
 * @param cpMousePathToSet Dateipfad des dem Objekt zugeordneten Maus-Devices, terminiert mit \0
 * @throw EXCEPTION_READ_MOUSE_ERROR Fehler beim Oeffnen des Maus-Devices
 *
 * setzt Dateipfad des dem Objekt zugeordneten Devices und oeffnet dieses read-only
 */
inputMouse::inputMouse(const char* cpMousePathToSet)
	: inputMovement(cpMousePathToSet) { /* Weitergabe von cpMousePathToSet an Konstruktor der inputMovement-Klasse */

	this->fds.events = POLLIN; /* Initialisiere fds.events mit POLLIN Bitmaske. POLLIN = pruefe ob einkommende Daten vorhanden */

	if ((this->fds.fd = open(this->cpDevicePath, O_RDONLY)) <= 0) { /* Oeffne Maus-Device-File read-only */
		throw EXCEPTION_READ_MOUSE_ERROR; /* Wenn Oeffnen nicht moeglich, werfe entsprechende Exception */
		this->fds.fd = 0; /* und setze Dateizeiger = 0 */
	}
}

/*
 * @brief Destruktor
 *
 * schliesst Dateizeiger
 */
inputMouse::~inputMouse() {
	close(this->fds.fd);
}

/*
 * @brief Methode zum Auslesen eines USB-Maus-Devices
 *
 * @return TRUE bei erfolgreichem Auslesen, FALSE bei Fehler
 */
bool inputMouse::read() {
	int iPollReturnValue = -1; /* Variable zum Speichern des Rueckgabewerts der poll()-Funktion */
	struct input_event ie; /* input_event struct des aktuellen Lesevorgangs */


	if (this->fds.fd > 0) { /* Wenn Datei in Konstruktor korrekt geoeffnet */
		iPollReturnValue = poll(&this->fds,1,POLLING_MOUSE_TIMEOUT_MS); /* Aufruf der poll()-Funktion mit Adresse der pollfd-struct, lese 1 struct und Timeout in ms */

		if (iPollReturnValue > 0) { /* Wenn polling erfolgreich, d.h. Daten anstehend */

			if (::read(this->fds.fd, &ie, sizeof(struct input_event)) <= 0) { /* Lese Inhalt des Dateizeigers in die input_event struct ie */
				throw EXCEPTION_READ_MOUSE_ERROR; /* Wenn Lesevorgang fehlschlaegt, werfe entsprechende Exception */
				return false; /* und gebe FALSE zurueck */
			}

			if (ie.type == EV_MSC) /* Da beim Einlesen von Klicks zwei Events auftreten, lese bei ie.type = EV_MSC(=4) nochmals */
				if (::read(this->fds.fd, &ie, sizeof(struct input_event)) <= 0) {
					throw EXCEPTION_READ_MOUSE_ERROR;
					return false;
				}

			/* Korrekt eingelesene Daten */

			/* Pruefe auf Linksklick */
			if (ie.type == EV_KEY && ie.code == BTN_LEFT)
				this->bBtn1 = ie.value; /* und schreibe Wert in eigenes Objekt */
			/* Pruefe auf Rechtsklick */
			if (ie.type == EV_KEY && ie.code == BTN_RIGHT)
				this->bBtn2 = ie.value; /* und schreibe Wert in eigenes Objekt */
			/* Pruefe auf Mittelklick */
			if (ie.type == EV_KEY && ie.code == BTN_MIDDLE)
				this->bBtn3 = ie.value; /* und schreibe Wert in eigenes Objekt */
			/* Pruefe auf X-Verschiebung */
			if (ie.type == EV_REL && ie.code == REL_X)
				this->iDX = ie.value; /* und schreibe Wert in eigenes Objekt */
			/* Pruefe auf Y-Verschiebung */
			if (ie.type == EV_REL && ie.code == REL_Y)
				this->iDY = ie.value; /* und schreibe Wert in eigenes Objekt */

		} /* if (iPollRetVal > 0) */

		if (iPollReturnValue == 0) { /* Wenn poll() mit Timeout beendet wird */
			this->iDX = 0; /* Setze X-Verschiebung = 0 */
			this->iDY = 0; /* Setze Y-Verschiebung = 0 */
		}

		if (iPollReturnValue < 0) { /* Wenn poll() mit Error beendet wird */
			throw EXCEPTION_POLLING_ERROR; /* werfe entsprechende Exception */
			return false; /* und gebe FALSE zurueck */
		}

		return true; /* da erfolgreich, gebe TRUE zurueck */

	} /* if (this->fds.fd > 0) Wenn Datei in Konstruktor korrekt geoeffnet */

	return false; /* In allen anderen Faellen gebe FALSE zurueck*/
} /* bool inputMouse::read() */

