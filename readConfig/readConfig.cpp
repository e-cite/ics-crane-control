/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: readConfig.cpp
 * Funktion: Liest die Werte der config.ini ein und schreibt diese in die struct configValues
 * Kommentar: Anpassungen im Kommentar, Exception-Codes
 * Name: Andreas Dolp
 * Datum: 17.05.2014
 * Version: 0.3
 ---------------------------*/

#include "readConfig.h"
#include <stdio.h> /* fopen, fgets, sscanf, fclose, printf */

/*
 * @brief Sucht in config-File nach String und gibt den dazugehoerigen config-Wert im Parameter zurueck
 * @param cpConfigFilePath Dateipfad der config-File
 * @param cpValueToFind String der den config-Eintrag beschreibt, terminiert mit \0
 * @param vpResult Speicherstelle des Result-Ergebnisses, muss von Aufrufer bereitgestellt werden
 * @return true bei Erfolg, false im Fehlerfall
 * @throw EXCEPTION_CPVALUETOFIND_NOT_FOUND Gesuchter config-Wert konnte nicht gefunden werden
 * @throw EXCEPTION_UNABLE_TO_OPEN_FILE Datei konnte nicht geoeffnet werden
 */
bool readConfigSingleValue(const char* cpConfigFilePath, const char* cpValueToFind, void* vpResult) {
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

/*
 * @brief Legt die im Header deklarierte struct configValues an und liest die spezifizierten Werte aus der config-File ein
 * @param cpConfigFilePath Dateipfad der config-File
 * @return configValues* als Pointer auf korrekt gefuellte struct bei Erfolg, NULL im Fehlerfall
 * @throw EXCEPTION_CPVALUETOFIND_NOT_FOUND Gesuchter config-Wert konnte nicht gefunden werden
 * @throw EXCEPTION_UNABLE_TO_OPEN_FILE Datei konnte nicht geoeffnet werden
 */
configValues* readConfig(const char* cpConfigFilePath) {
	/* Allokiere Speicher fuer struct configValues */
	configValues* configValuespResult = new configValues;

	/* Rufe readConfigSingleValue mit allen zu suchenden Werten auf */
	try {
		readConfigSingleValue( cpConfigFilePath,"mousePath=%s", (void*)configValuespResult->mousePath );
		readConfigSingleValue( cpConfigFilePath,"joystickPath=%s", (void*)configValuespResult->joystickPath );
		readConfigSingleValue( cpConfigFilePath,"gpioXF=%d", (void*)&configValuespResult->gpioXF );
		readConfigSingleValue( cpConfigFilePath,"gpioXB=%d", (void*)&configValuespResult->gpioXB );
		readConfigSingleValue( cpConfigFilePath,"gpioYF=%d", (void*)&configValuespResult->gpioYF );
		readConfigSingleValue( cpConfigFilePath,"gpioYB=%d", (void*)&configValuespResult->gpioYB );
		readConfigSingleValue( cpConfigFilePath,"gpioZF=%d", (void*)&configValuespResult->gpioZF );
		readConfigSingleValue( cpConfigFilePath,"gpioZB=%d", (void*)&configValuespResult->gpioZB );
		readConfigSingleValue( cpConfigFilePath,"gpioUSBError=%d", (void*)&configValuespResult->gpioUSBError );
	} catch (int e) {
		/* Pruefe gefangene Exceptions mit den in readConfig.h Definierten */
		if (e == EXCEPTION_CPVALUETOFIND_NOT_FOUND) {
			/* und gebe entsprechende Fehlermeldung aus */
			printf( "ERROR: At least one config-value not found!\n" );
			/* schreibe stdout-Buffer an stdout */
			fflush(stdout);
		}
		if (e == EXCEPTION_UNABLE_TO_OPEN_FILE) {
			/* und gebe entsprechende Fehlermeldung aus */
			printf( "ERROR: Unable to read config-file!\n" );
			/* schreibe stdout-Buffer an stdout */
			fflush(stdout);
		}
		/* Wenn beliebige Exception auftritt */
		if (e > 0) {
			/* gebe allokierten Speicher wieder frei */
			delete configValuespResult;
			/* gebe NULL-Pointer zurueck */
			return NULL;
		}
	}
	/* Wenn alle Werte korrekt gefunden, gebe Erfolgsmeldung aus */
	printf("SUCCESS: %s read correctly!\n", cpConfigFilePath);
	/* schreibe stdout-Buffer an stdout */
	fflush(stdout);
	/* und gebe Pointer auf gueltige struct zurueck */
	return configValuespResult;
}
