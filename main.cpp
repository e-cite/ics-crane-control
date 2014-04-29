/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: main.cpp
 * Funktion: Hauptprojekt
 * Kommentar: Anpassungen fuer Problembehebung "verlorene input-events", delete primMouse eingefuegt
 * Name: Andreas Dolp
 * Datum: 29.04.2014
 * Version: 0.1
 ---------------------------*/

#include "main.h"
#include <cstdio>	/* printf */
#include "input/inputMouse.h"	/* inputMouse* primMouse = */

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
// TODO Exceptions in main koennen noch nicht gefangen werden, da namespace dann nicht passt!
    inputMouse* primMouse = new inputMouse("/dev/input/event1");
    while (1) {
    	try {
    		primMouse->read();
    	} catch (int e) {
    		if (e >= EXCEPTION_POLLING_ERROR)
    			printf("POLLING ERROR\n");
    		if (e == EXCEPTION_POLLING_TIMEOUT)
    		    printf("Timeout while polling\n");
    	}
        printf("Links:	%d\n",primMouse->getClickLeft());
        printf("Rechts:	%d\n",primMouse->getClickRight());
        printf("Mitte:	%d\n",primMouse->getClickMiddle());
        printf("dX:	%d\n",primMouse->getDX());
        printf("dY:	%d\n",primMouse->getDY());
        fflush(stdout);
    }

    delete primMouse;
	return 0;
}


