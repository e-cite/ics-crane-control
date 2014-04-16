/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: inputMouse.h
 * Funktion: Header zu inputMouse.cpp, public-Ableitung der Klasse inputMouse von inputMovement. Definiert die noetigen Funktionen zum Einlesen einer USB-Maus
 * Kommentar: Erste Version
 * Parameter:
 * Rueckgabe:
 * Name: Andreas Dolp
 * Datum: 09.04.2014
 * Version: 0.1
 ---------------------------*/

#ifndef INPUTMOUSE_H_
#define INPUTMOUSE_H_

#include "inputMovement.h"


class inputMouse: public inputMovement {
public:
	inputMouse(const char* cpMousePathToSet);
	bool read();
};

#endif /* INPUTMOUSE_H_ */
