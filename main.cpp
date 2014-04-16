/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: main.cpp
 * Funktion: Hauptprojekt
 * Kommentar: Anpassungen fuer ersten Test auf RaspberryPi
 * Name: Andreas Dolp
 * Datum: 16.04.2014
 * Version: 0.1
 ---------------------------*/

#include "main.h"
#include <cstdio>	/* printf */
#include "inputMouse.h"	/* inputMouse* primMouse = */

int main ( int argc, char* argv[] ) {
	printf("ICS - Crane Control\n");
	printf("============================================================\n");
	printf("Version %G\n", VERSION);
	printf("============================================================\n");
	printf("Written by Andreas Dolp for ICS - Innovative Crane Solutions\n");
	printf("This software is licensed under GNU GPLv3\n");
	printf("============================================================\n");

    inputMouse* primMouse = new inputMouse("/dev/input/mice");
    while (primMouse->read()) {}
	return 0;
}


