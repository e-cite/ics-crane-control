/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: inputMouseGorloTodt.cpp
 * Funktion: Implementierung der Klasse inputMouseGorloTodt, Programmierung der Methoden
 * Kommentar: Erste Version
 * Name: Andreas Dolp
 * Datum: 16.07.2014
 * Version: 0.1
 ---------------------------*/

#include "inputMouseGorloTodt.h"
#include <fcntl.h> /* open */
#include <unistd.h> /* read, close */
#include <linux/input.h> /* struct input_event */
#include <poll.h> /* struct pollfd, poll */

/*
 * @brief Konstruktor
 * @param cpMousePathToSet Dateipfad des dem Objekt zugeordneten Maus-Devices, terminiert mit \0
 * @throw EXCEPTION_READ_MOUSE_GORLOTODT_ERROR Fehler beim Oeffnen des Maus-Devices
 *
 * setzt Dateipfad des dem Objekt zugeordneten Devices und oeffnet dieses read-only
 */
inputMouseGorloTodt::inputMouseGorloTodt(const char* cpMousePathToSet)
	: inputMouse(cpMousePathToSet) { /* Weitergabe von cpMousePathToSet an Konstruktor der inputMouse-Klasse */

	this->fds.events = POLLIN; /* Initialisiere fds.events mit POLLIN Bitmaske. POLLIN = pruefe ob einkommende Daten vorhanden */

	if ((this->fds.fd = open(this->cpDevicePath, O_RDONLY)) <= 0) { /* Oeffne Maus-Device-File read-only */
		throw EXCEPTION_READ_MOUSE_ERROR; /* Wenn Oeffnen nicht moeglich, werfe entsprechende Exception */
		this->fds.fd = 0; /* und setze Dateizeiger = 0 */
	}
	unsigned short id[4]; /* Array fuer die InputDevice-IDs */
	ioctl(this->fds.fd,EVIOCGID,id); /* Lese InputDevice-IDs in id-Array ein */

	/* Objekte dieser Art nur bei folgenden Eingabegeraeten zulassen */
	if(id[ID_VENDOR] == 0xa81 && id[ID_PRODUCT] == 0x205)
		return;

	/* Wenn nicht eines der oben definierten Geraete */
	throw EXCEPTION_READ_MOUSE_GORLOTODT_ERROR; /* werfe entsprechende Exception */
	this->fds.fd = 0; /* und setze Dateizeiger = 0 */
}

/*
 * @brief Destruktor
 *
 * schliesst Dateizeiger
 */
inputMouseGorloTodt::~inputMouseGorloTodt() {
	close(this->fds.fd);
}
