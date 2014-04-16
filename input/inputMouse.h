/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: inputMouse.h
 * Funktion: Header zu inputMouse.cpp, public-Ableitung der Klasse inputMouse von inputMovement. Definiert die noetigen Funktionen zum Einlesen einer USB-Maus
 * Kommentar: Fertigstellung des Konstruktors, erste komplette Programmierung der inputMouse::read()
 * Name: Andreas Dolp
 * Datum: 09.04.2014
 * Version: 0.1
 ---------------------------*/

#ifndef INPUTMOUSE_H_
#define INPUTMOUSE_H_

#define SIZE_OF_STRING_LENGTH 100
#define SIZE_OF_MOUSE_DATA 3

#define EXCEPTION_UNABLE_READ_MOUSE 1


#include "inputMovement.h"


class inputMouse: public inputMovement {
public:
	inputMouse(const char* cpMousePathToSet);
	bool read();

private:
	const char cpMousePath[SIZE_OF_STRING_LENGTH];
};

#endif /* INPUTMOUSE_H_ */
