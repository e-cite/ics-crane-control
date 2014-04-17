/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: inputMouse.h
 * Funktion: Header zu inputMouse.cpp, public-Ableitung der Klasse inputMouse von inputMovement. Definiert die noetigen Funktionen zum Einlesen einer USB-Maus
 * Kommentar: Anpassung an v0.3, Einbau der Exceptions fuer polling
 * Name: Andreas Dolp
 * Datum: 17.04.2014
 * Version: 0.3
 ---------------------------*/

#ifndef INPUTMOUSE_H_
#define INPUTMOUSE_H_

#define SIZE_OF_STRING_LENGTH 100
#define POLLING_TIMEOUT_MS 500

#define EXCEPTION_POLLING_TIMEOUT 1
#define EXCEPTION_POLLING_ERROR 2
#define EXCEPTION_UNABLE_READ_MOUSE 3

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
