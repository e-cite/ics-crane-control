/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: inputJoystick.cpp
 * Funktion: Implementierung der Klasse inputJoystick, Programmierung der Funktionen
 * Kommentar: Ueberarbeitungen, erste vollstaendig lauffaehige Version, Einbau Fehlerauswertung ::read-Funktion
 * Name: Andreas Dolp
 * Datum: 08.05.2014
 * Version: 1.0
 ---------------------------*/

#include "inputJoystick.h"
#include <fcntl.h> /* open */
#include <unistd.h> /* read, close */
#include <linux/input.h> /* struct input_event */

/*
 * Konstruktor
 * setzt Dateipfad des dem Objekt zugeordneten Devices und oeffnet dieses read-only
 * @param cpJoystickPathToSet Dateipfad des dem Objekt zugeordneten Joystick-Devices, terminiert mit \n
 * @except EXCEPTION_READ_JOYSTICK_ERROR Fehler beim Oeffnen des Joystick-Devices
 */
inputJoystick::inputJoystick(const char* cpJoystickPathToSet)
	: inputMovement(cpJoystickPathToSet) { /* Weitergabe von cpJoystickPathToSet an Konstruktor der inputMovement-Klasse */

	if ((this->fd = open(this->cpDevicePath, O_RDONLY)) <= NULL) { /* Oeffne Joystick-Device-File read-only */
		throw EXCEPTION_READ_JOYSTICK_ERROR; /* Wenn Oeffnen nicht moeglich, werfe entsprechende Exception */
		this->fd = NULL; /* setze Dateizeiger = NULL */
		return; /* und gebe zurueck */
	}

	struct input_absinfo ieAbsInfo;	/* struct input_absinfo fuer die Absolutwerte der Achsen */
	ioctl(this->fd,EVIOCGABS(ABS_X),&ieAbsInfo); /* Lese Absolutwerte fuer X-Achse in ieAbsInfo ein */
	this->iAxisXCentralAbsValue = (ieAbsInfo.maximum - ieAbsInfo.minimum) / 2; /* Verrechne Wert mit Minimum-Achswert und bilde so Absolutwert des Mittelpunkts */

	ioctl(this->fd,EVIOCGABS(ABS_Y),&ieAbsInfo); /* Lese Absolutwerte fuer Y-Achse in ieAbsInfo ein */
	this->iAxisYCentralAbsValue = (ieAbsInfo.maximum - ieAbsInfo.minimum) / 2; /* Verrechne Wert mit Minimum-Achswert und bilde so Absolutwert des Mittelpunkts */
}

/*
 * Destruktor
 * schliesst Dateizeiger
 */
inputJoystick::~inputJoystick() {
	close(this->fd);
}

/*
 * Funktion zum Auslesen eines USB-Joystick-Devices
 * @return TRUE bei erfolgreichem Auslesen, FALSE bei Fehler
 */
bool inputJoystick::read() {
	struct input_event ie; /* input_event struct des aktuellen Lesevorgangs */

	if (this->fd > NULL) { /* Wenn Datei in Konstruktor korrekt geoeffnet */
		if (::read(this->fd, &ie, sizeof(struct input_event)) <= 0) { /* Lese Inhalt des Dateizeigers in die input_event struct ie */
			throw EXCEPTION_READ_JOYSTICK_ERROR; /* Wenn Lesevorgang fehlschlaegt, werfe entsprechende Exception */
			return false; /* und gebe FALSE zurueck */
		}

		if (ie.type == EV_MSC) /* Da beim Einlesen von Klicks zwei Events auftreten, lese bei ie.type = EV_MSC(=4) nochmals */
			if (::read(this->fd, &ie, sizeof(struct input_event)) <= 0) {
				throw EXCEPTION_READ_JOYSTICK_ERROR;
				return false;
			}

		/* Korrekt eingelesene Daten */

		/* Pruefe auf Linksklick */
		if (ie.type == EV_KEY && ie.code == BTN_TRIGGER)
			this->bBtn1 = ie.value; /* und schreibe Wert in eigenes Objekt */
		/* Pruefe auf Rechtsklick */
		if (ie.type == EV_KEY && ie.code == BTN_THUMB)
			this->bBtn2 = ie.value; /* und schreibe Wert in eigenes Objekt */
		/* Pruefe auf Mittelklick */
		if (ie.type == EV_KEY && ie.code == BTN_THUMB2)
			this->bBtn3 = ie.value; /* und schreibe Wert in eigenes Objekt */
		/* Pruefe auf X-Verschiebung und berechne Wert relativ zum Mittelpunkt */
		if (ie.type == EV_ABS && ie.code == ABS_X)
			this->iDX = ie.value - this->iAxisXCentralAbsValue; /* und schreibe Wert in eigenes Objekt */
		/* Pruefe auf Y-Verschiebung und berechne Wert relativ zum Mittelpunkt */
		if (ie.type == EV_ABS && ie.code == ABS_Y)
			this->iDY = ie.value - this->iAxisYCentralAbsValue; /* und schreibe Wert in eigenes Objekt */

		return true; /* da erfolgreich, gebe TRUE zurueck */

	} /* if (this->fd > NULL) { Wenn Datei in Konstruktor korrekt geoeffnet */

	return false; /* In allen anderen Faellen gebe FALSE zurueck*/
} /* bool inputJoystick::read() */
