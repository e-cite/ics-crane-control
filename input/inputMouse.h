/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: inputMouse.h
 * Funktion: Header zu inputMouse.cpp, public-Ableitung der Klasse inputMouse von inputMovement. Definiert die noetigen Funktionen zum Einlesen einer USB-Maus
 * Kommentar: Anpassung auf Verallgemeinerung aus inputJoystick-Klasse
 * Name: Andreas Dolp
 * Datum: 06.05.2014
 * Version: 0.4
 ---------------------------*/

#ifndef INPUTMOUSE_H_
#define INPUTMOUSE_H_

#define POLLING_TIMEOUT_MS 100
// TODO Exception codes konvention
#define EXCEPTION_POLLING_TIMEOUT 1
#define EXCEPTION_POLLING_ERROR 2
#define EXCEPTION_UNABLE_READ_MOUSE 3

#include "inputMovement.h"
#include <poll.h>	/* struct pollfd, poll */


/* Ableitung der inputMouse-Klasse von der inputMovement-Klasse */
class inputMouse: public inputMovement {
public:
	inputMouse(const char* cpMousePathToSet);	/* Allgemeiner Konstruktor */
	~inputMouse();								/* Destruktor */
	bool read();								/* Funktion zum Auslesen eines Maus-Devices */

private:
	struct pollfd fds;	/* struct fuer file-descriptor, definiert zu ueberwachende poll-events */
};

#endif /* INPUTMOUSE_H_ */
