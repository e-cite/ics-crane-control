/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: outputGPIOsysfs.cpp
 * Funktion: Implementierung der Klasse outputGPIOsysfs, Programmierung der Funktionen
 * Kommentar: Fehlerverbesserungen in der init-Routine, aufgetreten durch RaspberryPi-Test
 * Name: Andreas Dolp
 * Datum: 24.04.2014
 * Version: 0.1
 ---------------------------*/

#include "outputGPIOsysfs.h"
#include <stdio.h>	/* fopen, fclose, fprintf, snprintf */

outputGPIOsysfs::outputGPIOsysfs(const unsigned int iaAdrGPIOPinsToSet[NUM_OF_SIGNALS]) {
	/* Initialisiere Adressen / Nummern der GPIO-Pins */
	for(int i = 0; i < NUM_OF_SIGNALS; i++) {
		iaAdrGPIOPins[i] = iaAdrGPIOPinsToSet[i];
	}

}

bool outputGPIOsysfs::init() {
	char caPath[MAX_PATH_LENGTH] = {'\0'}; /* Buffer fuer Pfadangaben */
	FILE* fp = NULL;	/* Dateizeiger */


/* EXPORT: Erstelle GPIO-Devices */
	snprintf(caPath, MAX_PATH_LENGTH, "%s%s", DEFAULT_GPIO_PATH, DEFAULT_GPIO_EXPORT_FILE); /* Baue Pfad zu Export-File zusammen */
	/* For-Schleife ueber alle verwendeten GPIO-Pins */
	for(int i = 0; i < NUM_OF_SIGNALS; i++) {
		fp = fopen(caPath, "w");	/* Oeffne Dateizeiger */
		/* Wenn Datei korrekt geoeffnet */
		if ( fp ) {
			/* Schreibe Werte in Export-File und pruefe auf Fehler */
			if( fprintf(fp, "%d", this->iaAdrGPIOPins[i]) <= 0 ) {
				/* Wenn Fehler aufgetreten, werfe Exception */
				throw EXCEPTION_ERROR_ACCESSING_EXPORT_FILE;
				fclose( fp );	/* Schliesse Dateizeiger auch bei Fehler */
				return false;
			}
			/* Schliesse Dateizeiger der Export-File */
			fclose( fp );
		} else {	/* wenn Datei nicht korrekt geoeffnet werden kann */
			throw EXCEPTION_ERROR_ACCESSING_EXPORT_FILE;	/* werfe entsprechende Exception */
			return false;	/* und gebe false zurueck */
		}
	}	/* for */


	/* Setze Buffer fuer Pfadangaben und File-Pointer zurueck */
	for (int i = 0; i < MAX_PATH_LENGTH; i++) caPath[i] = '\0';
	fp = NULL;


/* DIRECTION: Setze Direction der einzelnen GPIO-Pins */
	/* For-Schleife ueber alle verwendeten GPIO-Pins */
	for(int i = 0; i < NUM_OF_SIGNALS; i++) {
		snprintf(caPath, MAX_PATH_LENGTH, "%s%s%d%s", DEFAULT_GPIO_PATH, DEFAULT_GPIO_PIN_SUBFOLDER, this->iaAdrGPIOPins[i], DEFAULT_GPIO_PIN_DIRECTION_FILE); /* Baue Pfad zu Direction-File zusammen */
		fp = fopen(caPath, "w");	/* Oeffne Dateizeiger */
		/* Wenn Datei korrekt geoeffnet */
		if ( fp ) {
			/* Schreibe Werte in Direction-File und pruefe auf Fehler */
			if( fprintf(fp, "%s", DEFAULT_GPIO_PIN_DIRECTION_OUT) <= 0 ) {
				/* Wenn Fehler aufgetreten, werfe Exception */
				throw EXCEPTION_ERROR_ACCESSING_DIRECTION_FILE;
				fclose( fp );	/* Schliesse Dateizeiger auch bei Fehler */
				return false;
			}
			/* Schliesse Dateizeiger der Direction-File */
			fclose( fp );
		} else {	/* wenn Datei nicht korrekt geoeffnet werden kann */
			throw EXCEPTION_ERROR_ACCESSING_DIRECTION_FILE;	/* werfe entsprechende Exception */
			return false;	/* und gebe false zurueck */
		}
	}	/* for */

/* Initialisierung fertig, gebe true zurueck */
	return true;
}

bool outputGPIOsysfs::write() {
	char caPath[MAX_PATH_LENGTH] = {'\0'}; /* Buffer fuer Pfadangaben */
	FILE* fp = NULL;	/* Dateizeiger */
	/* For-Schleife ueber alle verwendeten GPIO-Pins */
	for(int i = 0; i < NUM_OF_SIGNALS; i++) {
		snprintf(caPath, MAX_PATH_LENGTH, "%s%s%d%s", DEFAULT_GPIO_PATH, DEFAULT_GPIO_PIN_SUBFOLDER, this->iaAdrGPIOPins[i], DEFAULT_GPIO_PIN_VALUE_FILE); /* Baue Pfad zu Value-File zusammen */
		fp = fopen(caPath, "w");	/* Oeffne Dateizeiger */
		/* Wenn Datei korrekt geoeffnet */
		if ( fp ) {
			/* Schreibe Werte in Value-File und pruefe auf Fehler */
			if( fprintf(fp, "%d", this->getSignal(i)) <= 0 ) {
				/* Wenn Fehler aufgetreten, werfe Exception */
				throw EXCEPTION_ERROR_ACCESSING_VALUE_FILE;
				fclose( fp );	/* Schliesse Dateizeiger auch bei Fehler */
				return false;
			}
			/* Schliesse Dateizeiger der Value-File */
			fclose( fp );
		} else {	/* wenn Datei nicht korrekt geoeffnet werden kann */
			throw EXCEPTION_ERROR_ACCESSING_VALUE_FILE;	/* werfe entsprechende Exception */
			return false;	/* und gebe false zurueck */
		}
	}	/* for */
	return false;
}
