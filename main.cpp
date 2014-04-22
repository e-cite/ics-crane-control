/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: main.cpp
 * Funktion: Hauptprojekt
 * Kommentar: Umbau und Anpassung fuer outputGPIO-Test
 * Name: Andreas Dolp
 * Datum: 22.04.2014
 * Version: 0.1
 ---------------------------*/

#include "main.h"
#include <cstdio>	/* printf */
#include "output/outputGPIOsysfs.h"
#include <unistd.h>	/* sleep */

int main ( int argc, char* argv[] ) {
	printf("ICS - Crane Control\n");
	printf("============================================================\n");
	printf("Version %G\n", VERSION);
	printf("============================================================\n");
	printf("Written by Andreas Dolp for ICS - Innovative Crane Solutions\n");
	printf("This software is licensed under GNU GPLv3\n");
	printf("============================================================\n");

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


