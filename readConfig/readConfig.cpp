/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: readConfig.cpp
 * Funktion: Liest die Werte der config.ini ein und schreibt diese in die struct configValues
 * Kommentar: Erste Version, Erste Implementierungen der Funktionen readConfig und readConfigSingleValue
 * Name: Andreas Dolp
 * Datum: 07.04.2014
 * Version: 0.1
 ---------------------------*/

#include "readConfig.h"
#include <stdio.h>		/* fopen, fgets, sscanf, fclose, printf */

bool readConfigSingleValue(char* cpConfigFilePath, char* cpValueToFind, void* vpResult) {
	/* Lesebuffer */
	char caBuffer[SIZE_OF_STRING_LENGTH];

	/* Oeffne Dateizeiger */
	FILE* fpConfigFile = fopen( cpConfigFilePath,"r-");

	/* Wenn Datei korrekt geoeffnet */
	if ( fpConfigFile ) {
		while ( !feof( fpConfigFile ) ) {
			/* Lese eine Zeile in Lesebuffer ein */
			fgets( caBuffer,sizeof(caBuffer),fpConfigFile );
			/* Wenn Wert gefunden */
			if ( sscanf( caBuffer,cpValueToFind,vpResult) ) {
				/* Schliesse Dateizeiger */
				fclose( fpConfigFile );
				/* Gebe true zurueck */
				return true;
			}
		} /* while */

		/* Wenn EOF erreicht und cpValueToFind nicht gefunden */
		/* Schliesse Dateizeiger */
		fclose( fpConfigFile );

		/* und werfe Exception, dass cpValueToFind nicht gefunden */
		throw EXCEPTION_CPVALUETOFIND_NOT_FOUND;


	} else {	/* wenn Datei nicht korrekt geoeffnet werden kann*/
		/* werfe Exception, dass Datei nicht geoeffnet werden kann */
		throw EXCEPTION_UNABLE_TO_OPEN_FILE;
	}
	/* und gebe false zurueck */
	return false;
}

configValues* readConfig(char* cpConfigFilePath) {
	/* Allokiere Speicher fuer struct configValues */
	configValues* result = new configValues;

	/* Rufe readConfigSingleValue mit allen zu suchenden Werten auf */
	readConfigSingleValue( cpConfigFilePath,"mousePath=%s", (void*)result->mousePath );
	return true;

	return false;
}



