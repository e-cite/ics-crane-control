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
#include <linux/input.h>	/* struct input_event */

using namespace std;

inputJoystick::inputJoystick(const char* cpJoystickPathToSet)
	: inputMovement(cpJoystickPathToSet) {
	/* Aufruf des allgemeinen Konstruktors der movementInput-Klasse */
	/* Oeffne Joystick-Device-File readonly */
	/* Wenn Oeffnen nicht moeglich */
	if ((fd = open(this->cpPath, O_RDONLY)) == -1)
		/* Werfe entsprechende Exception */
		throw EXCEPTION_UNABLE_READ_JOYSTICK;
	struct input_absinfo ieAbsInfo;	/* struct input_absinfo fuer die Absolutwerte der Achsen */
	/* Lese Absolutwerte fuer X-Achse in ieAbsInfo ein */
	ioctl(this->fd,EVIOCGABS(ABS_X),&ieAbsInfo);
	/* Verrechne den Wert mit dem Minimum und bilde so Absolutwert des Mittelpunkts */
	this->iAxisXCentralAbsValue = (ieAbsInfo.maximum - ieAbsInfo.minimum) / 2;
	/* Lese Absolutwerte fuer Y-Achse in ieAbsInfo ein */
	ioctl(this->fd,EVIOCGABS(ABS_Y),&ieAbsInfo);
	/* Verrechne den Wert mit dem Minimum und bilde so Absolutwert des Mittelpunkts */
	this->iAxisYCentralAbsValue = (ieAbsInfo.maximum - ieAbsInfo.minimum) / 2;
}

inputJoystick::~inputJoystick() {
	/* Schliesse Datei */
	close(fd);
}


bool inputJoystick::read() {
	/* input_event struct des aktuellen Lesevorgangs */
	struct input_event ie;

	/* Wenn Datei in Konstruktor korrekt geoeffnet */
	if (fd != -1) {
		/* Lese Inhalt des file-descriptors in die input_event struct ie */
		::read(fd, &ie, sizeof(struct input_event));
		/* Da beim Einlesen von Klicks zwei Events auftreten, lese bei ie.type = EV_MSC(=4) nochmals */
		if (ie.type == EV_MSC)
			::read(fd, &ie, sizeof(struct input_event));
		/* Pruefe auf Linksklick */
		if (ie.type == EV_KEY && ie.code == BTN_TRIGGER)
			this->bBtn1 = ie.value;
		/* Pruefe auf Rechtsklick */
		if (ie.type == EV_KEY && ie.code == BTN_THUMB)
			this->bBtn2 = ie.value;
		/* Pruefe auf Mittelklick */
		if (ie.type == EV_KEY && ie.code == BTN_THUMB2)
			this->bBtn3 = ie.value;
		/* Pruefe auf X-Verschiebung und Berechne Wert relativ zum Mittelpunkt */
		if (ie.type == EV_ABS && ie.code == ABS_X)
			this->iDX = ie.value - this->iAxisXCentralAbsValue;
		/* Pruefe auf Y-Verschiebung und Berechne Wert relativ zum Mittelpunkt */
		if (ie.type == EV_ABS && ie.code == ABS_Y)
				this->iDY = ie.value - this->iAxisYCentralAbsValue;

		/* da erfolgreich, gebe true zurueck */
		return true;

	} /* if Device korrekt geoeffnet */

	/* Wenn Lesen nicht moeglich gebe false zurueck */
	return false;
}
