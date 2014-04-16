/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: inputMouse.cpp
 * Funktion: Implementierung der Klasse inputMouse, Programmierung der Funktionen
 * Kommentar: Fertigstellung des Konstruktors, erste komplette Programmierung der inputMouse::read()
 * Name: Andreas Dolp
 * Datum: 09.04.2014
 * Version: 0.1
 ---------------------------*/

#include "inputMouse.h"
#include <fcntl.h>			/* open */
#include <unistd.h>			/* read, close */
#include <iostream>			/* cout */
using namespace std;

inputMouse::inputMouse(const char* cpMousePathToSet)
	: cpMousePath{*cpMousePathToSet} {
}


bool inputMouse::read() {
	int fd;
	char buffer[SIZE_OF_MOUSE_DATA];
	if (( fd = open(this->cpMousePath, O_RDONLY)) == -1) {
		throw EXCEPTION_UNABLE_READ_MOUSE;
		return false;
	} else {
		::read(fd, &buffer, SIZE_OF_MOUSE_DATA);
		this->bClickLeft = (buffer[0] & 0b00000001) >> 0;
		if ((buffer[0] & 0b00000001) >> 0)
			cout << "Maus links \n";

		this->bClickRight = (buffer[0] & 0b00000010) >> 1;
		if ((buffer[0] & 0b00000010) >> 1)
			cout << "Maus rechts \n";

		this->bClickMiddle = (buffer[0] & 0b00000100) >> 2;
		if ((buffer[0] & 0b00000100) >> 2)
			cout << "Maus mitte \n";
			cout << "X: " << buffer[1] << "\n";
			cout << "Y: " << buffer[2] << "\n";
		}
		close(fd);
	return false;
}

