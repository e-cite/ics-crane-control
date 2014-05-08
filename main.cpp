/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: main.cpp
 * Funktion: Hauptprojekt
 * Kommentar: Anpassungen nach Ueberarbeitungen
 * Name: Andreas Dolp
 * Datum: 08.05.2014
 * Version: 0.1
 ---------------------------*/

#include "main.h"
#include <cstdio>	/* printf */
#include "input/inputMouse.h"	/* inputMouse* primMouse = */
#include "input/inputJoystick.h"	/* inputJoystick* primJoystick = */

int main ( int argc, char* argv[] ) {
	printf("ICS - Crane Control\n");
	printf("============================================================\n");
#ifndef _DEBUG
	printf("Version %G\n", VERSION);
#else	/* _DEBUG */
	printf("Version %G DEBUG\n", VERSION);
#endif	/* _DEBUG */
	printf("============================================================\n");
	printf("Written by Andreas Dolp for ICS - Innovative Crane Solutions\n");
	printf("This software is licensed under GNU GPLv3\n");
	printf("============================================================\n");

    inputMouse* primMouse = new inputMouse("/dev/input/event1");
//    inputJoystick* primJoystick = new inputJoystick("/dev/input/event0");
    inputMovement* myDevice;
    myDevice = primMouse;
    while (1) {
    	try {
    		myDevice->read();
    	} catch (int e) {
    		if (e < 0) {
    			printf("ERROR: %d\n",e);
    			return -1;
    		}
    	}
        printf("Links:	%d\n",myDevice->getBtn1());
        printf("Rechts:	%d\n",myDevice->getBtn2());
        printf("Mitte:	%d\n",myDevice->getBtn3());
        printf("dX:	%d\n",myDevice->getDX());
        printf("dY:	%d\n",myDevice->getDY());
        fflush(stdout);
    }

    delete primMouse;
//    delete primJoystick;
	return 0;
}


