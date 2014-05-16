/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: inputJoystick.h
 * Funktion: Header zu inputJoystick.cpp, Deklaration der Klasse inputJoystick
 * Kommentar: Ueberarbeitungen, erste vollstaendig lauffaehige Version
 * Name: Andreas Dolp
 * Datum: 16.05.2014
 * Version: 1.1
 ---------------------------*/

#ifndef INPUTJOYSTICK_H_
#define INPUTJOYSTICK_H_

#define EXCEPTION_READ_JOYSTICK_ERROR -12

#include "inputMovement.h"

/*
 * Deklaration der Klasse inputJoystick als public-Ableitung von inputMovement
 * Implementiert die Methoden zum Einlesen eines USB-Joysticks
 */
class inputJoystick: public inputMovement {
public:
	inputJoystick(const char*); /* Konstruktor */
	~inputJoystick(); /* Destruktor */
	bool read(); /* Methode zum Auslesen eines USB-Joystick-Devices */

private:
	int fd; /* Dateizeiger des dem Objekt zugeordneten Joystick-Devices */
	int iAxisXCentralAbsValue; /* Absoluter X-Wert der Mittelpunkts-Stellung */
	int iAxisYCentralAbsValue; /* Absoluter Y-Wert der Mittelpunkts-Stellung */
};

#endif /* INPUTJOYSTICK_H_ */
