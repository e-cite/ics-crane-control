/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: inputMouse.h
 * Funktion: Header zu inputMouse.cpp, Definition der Klasse inputMouse
 * Kommentar: Ueberarbeitungen, erste vollstaendig lauffaehige Version
 * Name: Andreas Dolp
 * Datum: 08.05.2014
 * Version: 1.0
 ---------------------------*/

#ifndef INPUTMOUSE_H_
#define INPUTMOUSE_H_

#define POLLING_MOUSE_TIMEOUT_MS 100

#define EXCEPTION_POLLING_ERROR -10
#define EXCEPTION_READ_MOUSE_ERROR -11

#include "inputMovement.h"
#include <poll.h> /* struct pollfd, poll */

/*
 * Definition der Klasse inputMouse als public-Ableitung von inputMovement
 * Definiert die Funktionen zum Einlesen einer USB-Maus
 */
class inputMouse: public inputMovement {
public:
	inputMouse(const char*); /* Konstruktor */
	~inputMouse(); /* Destruktor */
	bool read(); /* Funktion zum Auslesen eines USB-Maus-Devices */

private:
	struct pollfd fds; /* struct fuer Dateizeiger, definiert auch die zu ueberwachenden Poll-Events */
};

#endif /* INPUTMOUSE_H_ */
