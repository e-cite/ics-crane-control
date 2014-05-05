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

#define SIZE_OF_STRING_LENGTH 100
#define POLLING_TIMEOUT_MS 100

#define EXCEPTION_POLLING_TIMEOUT 1
#define EXCEPTION_POLLING_ERROR 2
#define EXCEPTION_UNABLE_READ_JOYSTICK 3

#include "inputMovement.h"
#include <poll.h>	/* struct pollfd, poll */


/* Ableitung der inputJoystick-Klasse von der inputMovement-Klasse */
class inputJoystick: public inputMovement {
public:
	inputJoystick(const char* cpJoystickPathToSet);	/* Allgemeiner Konstruktor */
	~inputJoystick();	/* Destruktor */
	bool read();	/* Funktion zum Auslesen eines Joystick-Devices */

private:
	char cpJoystickPath[SIZE_OF_STRING_LENGTH];	/* Dateipfad des dem Objekt zugeordneten Devices */
	struct pollfd fds;	/* struct fuer file-descriptor, definiert zu ueberwachende poll-events */
};

#endif /* INPUTJOYSTICK_H_ */
