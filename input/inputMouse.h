/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: inputMouse.h
 * Funktion: Header zu inputMouse.cpp, public-Ableitung der Klasse inputMouse von inputMovement. Definiert die noetigen Funktionen zum Einlesen einer USB-Maus
 * Kommentar: Anpassung an v0.2, Entfernung von caRawInput-Data und SIZE_OF_MOUSE_DATA
 * Name: Andreas Dolp
 * Datum: 16.04.2014
 * Version: 0.2
 ---------------------------*/

#ifndef INPUTMOUSE_H_
#define INPUTMOUSE_H_

#define SIZE_OF_STRING_LENGTH 100

#define EXCEPTION_UNABLE_READ_MOUSE 1


#include "inputMovement.h"


/* Ableitung der inputMouse-Klasse von der inputMovement-Klasse */
class inputMouse: public inputMovement {
public:
	inputMouse(const char* cpMousePathToSet);	/* Allgemeiner Konstruktor */
	bool read();								/* Funktion zum Auslesen eines Maus-Devices */

private:
	char cpMousePath[SIZE_OF_STRING_LENGTH];	/* Dateipfad des dem Objekt zugeordneten Devices */
};

#endif /* INPUTMOUSE_H_ */
