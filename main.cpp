/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: main.cpp
 * Funktion: Hauptprojekt
 * Kommentar: Anpassungen fuer input- und output-Test
 * Name: Andreas Dolp
 * Datum: 24.04.2014
 * Version: 0.1
 ---------------------------*/

#include "main.h"
#include "input/inputMouse.h"	/* inputMouse* primMouse = */
#include "output/outputGPIOsysfs.h"
#include <cstdio>	/* printf */
#include <unistd.h>	/* sleep */


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


	unsigned int iaMyGPIOAddresses[NUM_OF_SIGNALS] = {17,27,22,10,9,11,7};
	outputGPIOsysfs* foo = new outputGPIOsysfs(iaMyGPIOAddresses);
	bool baMySignalsToSet[NUM_OF_SIGNALS] = {0,0,0,0,0,0,0};
	try {
		foo->init();
	} catch (int e) {
		printf("Initialization Errors: %d\n", e);
		fflush(stdout);
	}
	printf("Initialized...\n");
	fflush(stdout);
	try {
		foo->write();
	} catch (int e) {
		printf("Writing Errors: %d\n", e);
		fflush(stdout);
	}
	printf("Write...\n");
	fflush(stdout);
	sleep(10);

	baMySignalsToSet[0] = 1;
	baMySignalsToSet[1] = 0;
	baMySignalsToSet[2] = 1;
	baMySignalsToSet[3] = 0;
	baMySignalsToSet[4] = 1;
	baMySignalsToSet[5] = 0;
	baMySignalsToSet[6] = 1;
	try {
		foo->setSignals(baMySignalsToSet);
	} catch (int e) {
		printf("Setting Errors: %d\n", e);
		fflush(stdout);
	}
	try {
		foo->write();
	} catch (int e) {
		printf("Writing Errors: %d\n", e);
		fflush(stdout);
	}
	printf("Write...\n");
	fflush(stdout);
	sleep(10);

	baMySignalsToSet[0] = 1;
	baMySignalsToSet[1] = 1;
	baMySignalsToSet[2] = 0;
	baMySignalsToSet[3] = 0;
	baMySignalsToSet[4] = 1;
	baMySignalsToSet[5] = 0;
	baMySignalsToSet[6] = 1;
	try {
		foo->setSignals(baMySignalsToSet);
	} catch (int e) {
		printf("Setting Errors: %d\n", e);
		fflush(stdout);
	}
	try {
		foo->write();
	} catch (int e) {
		printf("Writing Errors: %d\n", e);
		fflush(stdout);
	}
	printf("Write...\n");
	fflush(stdout);
	sleep(10);

	return 0;
}


