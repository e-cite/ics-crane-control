/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: inputJoystick.h
 * Funktion: Header zu inputJoystick.cpp, public-Ableitung der Klasse inputJoystick von inputMovement. Definiert die noetigen Funktionen zum Einlesen eines USB-Joysticks
 * Kommentar: Erste Version, basierend auf inputMouse 0.4
 * Name: Andreas Dolp
 * Datum: 06.05.2014
 * Version: 0.1
 ---------------------------*/

#ifndef INPUTJOYSTICK_H_
#define INPUTJOYSTICK_H_

#define POLLING_TIMEOUT_MS 100
// TODO Exception codes konvention
#define EXCEPTION_UNABLE_READ_JOYSTICK 1

#include "inputMovement.h"


/* Ableitung der inputJoystick-Klasse von der inputMovement-Klasse */
class inputJoystick: public inputMovement {
public:
	inputJoystick(const char* cpJoystickPathToSet);	/* Allgemeiner Konstruktor */
	~inputJoystick();	/* Destruktor */
	bool read();	/* Funktion zum Auslesen eines Joystick-Devices */

private:
	int fd;	/* Dateipfad des dem Objekt zugeordneten Devices */
	int iAxisXCentralAbsValue;	/* Absoluter X-Wert der Mittelpunkts-Stellung */
	int iAxisYCentralAbsValue;	/* Absoluter Y-Wert der Mittelpunkts-Stellung */
};

#endif /* INPUTJOYSTICK_H_ */
