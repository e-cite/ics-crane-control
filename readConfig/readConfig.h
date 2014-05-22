/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: readConfig.h
 * Funktion: Header zu readConfig.cpp, Deklariert die struct configValues
 * Kommentar: Umbau des Konzepts: Result-Struct muss von Aufrufer allokiert werden
 * Name: Andreas Dolp
 * Datum: 22.05.2014
 * Version: 1.1
 ---------------------------*/

#ifndef READCONFIG_H_
#define READCONFIG_H_

#include <stdio.h> /* FILE* */

#define SIZE_OF_STRING_LENGTH 100
#define NUM_OF_CONFIG_PARAMETERS 14

/*
 * @brief Deklariere struct configValues
 *
 * Diese beinhaltet alle aus der config.ini eingelesenen Werte
 */
typedef struct configValues {
	 char caDevicePath[SIZE_OF_STRING_LENGTH];
	 unsigned int iGpioUSBError;
	 unsigned int iGpioXF;
	 unsigned int iGpioXB;
	 unsigned int iGpioYF;
	 unsigned int iGpioYB;
	 unsigned int iGpioZF;
	 unsigned int iGpioZB;
	 int iDeltaRelXMin;
	 int iDeltaRelYMin;
	 int iDeltaAbsXMin;
	 int iDeltaAbsYMin;
	 int iFlagDebug;
	 int iFlagNcurses;
} configValues;

int readConfigSingleValue(FILE*, const char*, void*); /* Prototyp */
int readConfig(configValues*, const char*); /* Prototyp */

#endif /* READCONFIG_H_ */
