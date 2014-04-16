/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: inputMouse.h
 * Funktion: Header zu inputMouse.cpp, public-Ableitung der Klasse inputMouse von inputMovement. Definiert die noetigen Funktionen zum Einlesen einer USB-Maus
 * Kommentar: Änderung des Einleseverfahrens, Anpassungen dazu in der inputMouse::read()-Funktion, Umbau des Konstruktors, Umstellung auf printf()-Ausgabe
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


/* Ableitung der inputMouse-Klasse auf der inputMovement-Klasse */
class inputMouse: public inputMovement {
public:
	inputMouse(const char* cpMousePathToSet);	/* Allgemeiner Konstruktor */
	bool read();								/* Funktion zum Auslesen eines Maus-Devices */

private:
	char cpMousePath[SIZE_OF_STRING_LENGTH];	/* Dateipfad des dem Objekt zugeordneten Devices */
	char caRawInput[SIZE_OF_MOUSE_DATA];		/* Rohdaten des aktuellen Lesevorgangs */
};

#endif /* INPUTMOUSE_H_ */
