/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: readConfig.c
 * Funktion: Liest Werte aus Config-File ein und schreibt diese an die uebergebene configValues* Speicherstelle
 * Kommentar: Nachbesserungen um C++-Kompatibilitaet zu gewaehrleisten
 * Name: Andreas Dolp
 * Datum: 23.05.2014
 * Version: 1.1
 ---------------------------*/

#include "readConfig.h"
#include <stdio.h> /* fopen, fgets, sscanf, fclose */

/*
 * @brief Sucht in Dateizeiger nach String und gibt den dazugehoerigen config-Wert im Parameter zurueck
 * @param filepConfigFile Dateizeiger zu geoeffneter config-File
 * @param cpValueToFind Suchstring im scanf-Format der den config-Eintrag sowie auszulesenden Wert beschreibt, terminiert mit \0
 * @param vpResult Speicherstelle des Result-Ergebnisses, muss von Aufrufer bereitgestellt werden
 * @return 1: Wert gefunden, 0: Wert nicht gefunden, -1: Fehler
 */
int readConfigSingleValue(FILE* filepConfigFile, const char* cpValueToFind, void* vpResult) {
	if ( filepConfigFile && cpValueToFind && vpResult ) { /* Wenn alle Parameter gueltig */
		char caBuffer[SIZE_OF_STRING_LENGTH]; /* Lesebuffer */

		rewind(filepConfigFile); /* Setze Dateizeiger auf Beginn der Datei */
		while ( !feof( filepConfigFile ) ) {
			fgets( caBuffer,sizeof(caBuffer),filepConfigFile ); /* Lese eine Zeile in Lesebuffer ein */
			if ( sscanf( caBuffer,cpValueToFind,vpResult) ) { /* Wenn Wert gefunden */
				return 1; /* Gebe 1 zurueck */
			}
		} /* while */
		/* Wenn keinen Wert gefunden, jedoch kein Fehler aufgetreten */
		return 0; /* gebe 0 zurueck */
	}
	return -1; /* und gebe -1 zurueck */
}

/*
 * @brief Liest die spezifizierten Werte aus der config-File ein und schreibt diese in bereitgestellte struct
 * @param configValuespResult Zeiger auf gueltig allokierte struct configValues
 * @param cpConfigFilePath Dateipfad der config-File, terminiert mit \0
 * @return 0: Alle Werte erfolgreich gelesen, > 0: Jedes mit 1 markierte Bit bedeutet dass dieser Wert nicht gefunden wurde, -1: Kein gueltiger Pointer uebergeben, -2: Config-File konnte nicht geoeffnet werden
 */
int readConfig(configValues* configValuespResult, const char* cpConfigFilePath) {
	if ( configValuespResult && cpConfigFilePath ) { /* Pruefe auf gueltige Pointer */
		unsigned short int iReturnValue = 0; /* Rueckgabe-Wert der Funktion */
		/* Array fuer Config-Parameter: [][0] = Suchstring im scanf-Format, [][1] = Zeiger auf Speicherstelle */
		void* vpaConfigParam[NUM_OF_CONFIG_PARAMETERS][2] = {
			{(void*)"devicePath=%s",(void*)configValuespResult->caDevicePath},
			{(void*)"gpioUSBError=%d",(void*)&configValuespResult->iGpioUSBError},
			{(void*)"gpioXF=%d",(void*)&configValuespResult->iGpioXF},
			{(void*)"gpioXB=%d",(void*)&configValuespResult->iGpioXB},
			{(void*)"gpioYF=%d",(void*)&configValuespResult->iGpioYF},
			{(void*)"gpioYB=%d",(void*)&configValuespResult->iGpioYB},
			{(void*)"gpioZF=%d",(void*)&configValuespResult->iGpioZF},
			{(void*)"gpioZB=%d",(void*)&configValuespResult->iGpioZB},
			{(void*)"deltaRelXMin=%d",(void*)&configValuespResult->iDeltaRelXMin},
			{(void*)"deltaRelYMin=%d",(void*)&configValuespResult->iDeltaRelYMin},
			{(void*)"deltaAbsXMin=%d",(void*)&configValuespResult->iDeltaAbsXMin},
			{(void*)"deltaAbsYMin=%d",(void*)&configValuespResult->iDeltaAbsYMin},
			{(void*)"flagDebug=%d",(void*)&configValuespResult->iFlagDebug},
			{(void*)"flagNcurses=%d",(void*)&configValuespResult->iFlagNcurses}
		};

		FILE* filepConfigFile = fopen( cpConfigFilePath,"r-"); /* Oeffne Datei */

		if ( filepConfigFile ) { /* Wenn Dateizeiger gueltig, d.h. Datei korrekt geoeffnet */
			/* Rufe readConfigSingleValue mit allen zu suchenden Werten auf */
			int i, iReadConfigSingleValueReturnValue;
			for(i = 0; i < NUM_OF_CONFIG_PARAMETERS; i++) {
				iReadConfigSingleValueReturnValue = readConfigSingleValue( filepConfigFile,(char*)vpaConfigParam[i][0], vpaConfigParam[i][1] );
				if (iReadConfigSingleValueReturnValue == 0) /* Wenn Wert nicht gefunden */
					iReturnValue = iReturnValue | (1<<i); /* Setze entsprechendes Bit in int-Wert */
				if (iReadConfigSingleValueReturnValue < 0) /* Wenn Fehler in readConfigSingleValue */
					break; /* Breche Schleife ab */
			}

			fclose (filepConfigFile); /* Schließe Dateizeiger */
			if (iReadConfigSingleValueReturnValue < 0) { /* Wenn Fehler in readConfigSingleValue aufgetreten */
				return -2; /* Gebe -2 zurueck */
			}
			return iReturnValue; /* Sonst gebe iReturnValue zurueck */

		}  else /* if ( filepConfigFile ) */
			return -2; /* Wenn Datei nicht korrekt geoeffnet, gebe -2 zurueck */

	} /* if ( configValuespResult && cpConfigFilePath ) */
	return -1; /* Wenn kein gueltiger Pointer fuer Ergebnis-struct und Dateipfad der Config-File uebergeben wurde, gebe -1 zurueck */
}
