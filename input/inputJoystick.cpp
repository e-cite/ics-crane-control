/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: inputJoystick.cpp
 * Funktion: Implementierung der Klasse inputJoystick, Programmierung der Methoden
 * Kommentar: Einbau der calculateThreshold-Methode zur Berechnung der Schwellwerte
 * Name: Andreas Dolp
 * Datum: 23.05.2014
 * Version: 1.2
 ---------------------------*/

#include "inputJoystick.h"
#include <fcntl.h> /* open */
#include <unistd.h> /* read, close */
#include <linux/input.h> /* struct input_event */

/*
 * @brief Konstruktor
 * @param cpJoystickPathToSet Dateipfad des dem Objekt zugeordneten Joystick-Devices, terminiert mit \0
 * @throw EXCEPTION_READ_JOYSTICK_ERROR Fehler beim Oeffnen des Joystick-Devices
 *
 * setzt Dateipfad des dem Objekt zugeordneten Devices und oeffnet dieses read-only
 */
inputJoystick::inputJoystick(const char* cpJoystickPathToSet)
	: inputMovement(cpJoystickPathToSet) { /* Weitergabe von cpJoystickPathToSet an Konstruktor der inputMovement-Klasse */

	if ((this->fd = open(this->cpDevicePath, O_RDONLY)) <= 0) { /* Oeffne Joystick-Device-File read-only */
		throw EXCEPTION_READ_JOYSTICK_ERROR; /* Wenn Oeffnen nicht moeglich, werfe entsprechende Exception */
		this->fd = 0; /* setze Dateizeiger = 0 */
		return; /* und gebe zurueck */
	}

	struct input_absinfo ieAbsInfo;	/* struct input_absinfo fuer die Absolutwerte der Achsen */
	ioctl(this->fd,EVIOCGABS(ABS_X),&ieAbsInfo); /* Lese Absolutwerte fuer X-Achse in ieAbsInfo ein */
	this->iAxisXCentralAbsValue = (ieAbsInfo.maximum - ieAbsInfo.minimum) / 2; /* Verrechne Wert mit Minimum-Achswert und bilde so Absolutwert des Mittelpunkts */

	ioctl(this->fd,EVIOCGABS(ABS_Y),&ieAbsInfo); /* Lese Absolutwerte fuer Y-Achse in ieAbsInfo ein */
	this->iAxisYCentralAbsValue = (ieAbsInfo.maximum - ieAbsInfo.minimum) / 2; /* Verrechne Wert mit Minimum-Achswert und bilde so Absolutwert des Mittelpunkts */
}

/*
 * @brief Destruktor
 *
 * schliesst Dateizeiger
 */
inputJoystick::~inputJoystick() {
	close(this->fd);
}

/*
 * @brief Methode zur Berechnung der Schwellwerte
 * @param ipThresholdX Pointer auf Speicherstelle des X-Schwellwertes in Prozent. An diese Stelle wird berechneter Absolut-Schwellwert geschrieben!
 * @param ipThresholdX Pointer auf Speicherstelle des Y-Schwellwertes in Prozent. An diese Stelle wird berechneter Absolut-Schwellwert geschrieben!
 */
void inputJoystick::calculateThreshold(int* ipThresholdX, int* ipThresholdY) {
	*ipThresholdX = this->iAxisXCentralAbsValue * *ipThresholdX / 100;
	*ipThresholdY = this->iAxisYCentralAbsValue * *ipThresholdY / 100;
}

/*
 * @brief Methode zum Auslesen eines USB-Joystick-Devices
 * @return TRUE bei erfolgreichem Auslesen, FALSE bei Fehler
 */
bool inputJoystick::read() {
	struct input_event ie; /* input_event struct des aktuellen Lesevorgangs */

	if (this->fd > 0) { /* Wenn Datei in Konstruktor korrekt geoeffnet */
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

	} /* if (this->fd > 0) { Wenn Datei in Konstruktor korrekt geoeffnet */

	return false; /* In allen anderen Faellen gebe FALSE zurueck*/
} /* bool inputJoystick::read() */
