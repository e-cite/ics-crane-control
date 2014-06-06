/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: outputGPIOsysfs.cpp
 * Funktion: Implementierung der Klasse outputGPIOsysfs, Programmierung der Methoden
 * Kommentar: Anpassungen der Wartezeit beim Erstellen der GPIO-Ausgaenge
 * Name: Andreas Dolp
 * Datum: 05.06.2014
 * Version: 1.1
 ---------------------------*/

#include "outputGPIOsysfs.h"
#include <cstdio> /* fopen, fclose, fprintf, snprintf */
#include <unistd.h> /* sleep */

/*
 * @brief Konstruktor
 * @param iaGPIOPinsAddressToSet Adresse / Nummern der GPIO-Pins(bzgl. Hardware-Pin-Map, nicht PIN-Nummer)
 *
 * Initialisiere Adresse / Nummern der GPIO-Pins
 */
outputGPIOsysfs::outputGPIOsysfs(const unsigned int iaGPIOPinsAddressToSet[NUM_OF_SIGNALS]) {
	for(int i = 0; i < NUM_OF_SIGNALS; i++) { /* Schleife ueber alle Signale */
		this->iaGPIOPinsAddress[i] = iaGPIOPinsAddressToSet[i]; /* Schreibe Wert in eigenes Objekt */
	}
}

/*
 * @brief Destruktor
 *
 * Setzt alle Signale auf LOW, inkl. USB-Fehler
 * Entspricht Aufruf der setUSBErrActive()-Methode mit anschliessendem Schreiben der Ausgaenge
 */
outputGPIOsysfs::~outputGPIOsysfs() {
	for (int i = 0; i < NUM_OF_SIGNALS; i++)
		this->iaGPIOPinsAddress[i] = 0; /* Setze alle Signale = 0 */
	this->write(); /* Schreibe Signale an Ausgaenge */
}

/*
 * @brief Methode zum Initialisieren der GPIO-Ausgaenge
 * @return TRUE wenn alle Ausgaenge erfolgreich initialisiert, sonst FALSE
 * @throw EXCEPTION_ERROR_ACCESSING_EXPORT_FILE Fehler bei schreibendem Zugriff auf Export-File
 * @throw EXCEPTION_ERROR_ACCESSING_DIRECTION_FILE Fehler bei schreibendem Zugriff auf Direction-File
 */
bool outputGPIOsysfs::init() {
	char caPathBuffer[MAX_PATH_LENGTH_OUTPUTGPIO] = {'\0'}; /* Buffer fuer Pfadangaben */
	FILE* fp = 0; /* Dateizeiger fuer GPIO-Devices */

/* EXPORT: Erstelle GPIO-Devices */
	snprintf(caPathBuffer, MAX_PATH_LENGTH_OUTPUTGPIO, "%s%s", DEFAULT_GPIO_PATH, DEFAULT_GPIO_EXPORT_FILE); /* Baue Pfad zu Export-File zusammen */
	for(int i = 0; i < NUM_OF_SIGNALS; i++) { /* Schleife ueber alle verwendeten Signale / GPIO-Pins */
		fp = fopen(caPathBuffer, "w"); /* Oeffne Dateizeiger read-write-able */
		if ( fp ) { /* Wenn Datei korrekt geoeffnet */
			if( fprintf(fp, "%d", this->iaGPIOPinsAddress[i]) <= 0 ) { /* Schreibe Werte in Export-File und pruefe auf Fehler */
				throw EXCEPTION_ERROR_ACCESSING_EXPORT_FILE; /* Wenn Fehler aufgetreten, werfe entsprechende Exception */
				fclose( fp ); /* Schliesse Dateizeiger auch bei Fehler */
				return false; /* und gebe FALSE zurueck */
			}
			fclose( fp ); /* Schliesse Dateizeiger der Export-File */
		} else { /* wenn Datei nicht korrekt geoeffnet */
			throw EXCEPTION_ERROR_ACCESSING_EXPORT_FILE; /* werfe entsprechende Exception */
			return false; /* und gebe FALSE zurueck */
		}
	} /* for(int i = 0; i < NUM_OF_SIGNALS; i++) */

	sleep(1); /* Warte vollstaendige Erzeugung der GPIO-Devices ab */
	/* Setze Buffer fuer Pfadangaben und Dateizeiger fuer GPIO-Devices zurueck */
	for (int i = 0; i < MAX_PATH_LENGTH_OUTPUTGPIO; i++) caPathBuffer[i] = '\0';
	fp = 0;

/* DIRECTION: Setze Direction der einzelnen GPIO-Pins */
	for(int i = 0; i < NUM_OF_SIGNALS; i++) { /* Schleife ueber alle verwendeten Signale / GPIO-Pins */
		snprintf(caPathBuffer, MAX_PATH_LENGTH_OUTPUTGPIO, "%s%s%d%s", DEFAULT_GPIO_PATH, DEFAULT_GPIO_PIN_SUBFOLDER, this->iaGPIOPinsAddress[i], DEFAULT_GPIO_PIN_DIRECTION_FILE); /* Baue Pfad zu Direction-File zusammen */
		fp = fopen(caPathBuffer, "w"); /* Oeffne Dateizeiger read-write-able */
		if ( fp ) { /* Wenn Datei korrekt geoeffnet */
			if( fprintf(fp, "%s", DEFAULT_GPIO_PIN_DIRECTION_OUT) <= 0 ) { /* Schreibe Werte in Direction-File und pruefe auf Fehler */
				throw EXCEPTION_ERROR_ACCESSING_DIRECTION_FILE; /* Wenn Fehler aufgetreten, werfe entsprechende Exception */
				fclose( fp ); /* Schliesse Dateizeiger auch bei Fehler */
				return false; /* und gebe FALSE zurueck */
			}
			fclose( fp ); /* Schliesse Dateizeiger der Direction-File */
		} else { /* wenn Datei nicht korrekt geoeffnet */
			throw EXCEPTION_ERROR_ACCESSING_DIRECTION_FILE;	/* werfe entsprechende Exception */
			return false; /* und gebe FALSE zurueck */
		}
	} /* for(int i = 0; i < NUM_OF_SIGNALS; i++) */

	return true; /* Initialisierung korrekt abgelaufen, gebe TRUE zurueck */
} /* bool outputGPIOsysfs::init() */

/*
 * @brief Methode zum Schreiben der gesetzten Signale an die GPIO-Ausgaenge
 * @return TRUE wenn alle Ausgaenge erfolgreich geschrieben, sonst FALSE
 * @throw EXCEPTION_ERROR_ACCESSING_VALUE_FILE Fehler bei schreibendem Zugriff auf Value-File
 */
bool outputGPIOsysfs::write() {
	char caPathBuffer[MAX_PATH_LENGTH_OUTPUTGPIO] = {'\0'}; /* Buffer fuer Pfadangaben */
	FILE* fp = 0; /* Dateizeiger fuer GPIO-Devices */

	for(int i = 0; i < NUM_OF_SIGNALS; i++) { /* Schleife ueber alle verwendeten Signale / GPIO-Pins */
		snprintf(caPathBuffer, MAX_PATH_LENGTH_OUTPUTGPIO, "%s%s%d%s", DEFAULT_GPIO_PATH, DEFAULT_GPIO_PIN_SUBFOLDER, this->iaGPIOPinsAddress[i], DEFAULT_GPIO_PIN_VALUE_FILE); /* Baue Pfad zu Value-File zusammen */
		fp = fopen(caPathBuffer, "w"); /* Oeffne Dateizeiger read-write-able */
		if ( fp ) { /* Wenn Datei korrekt geoeffnet */
			if( fprintf(fp, "%d", this->getSignal(i)) <= 0 ) { /* Schreibe Werte in Value-File und pruefe auf Fehler */
				throw EXCEPTION_ERROR_ACCESSING_VALUE_FILE; /* Wenn Fehler aufgetreten, werfe entsprechende Exception */
				fclose( fp ); /* Schliesse Dateizeiger auch bei Fehler */
				return false; /* und gebe FALSE zurueck */
			}
			fclose( fp ); /* Schliesse Dateizeiger der Value-File */
		} else { /* wenn Datei nicht korrekt geoeffnet */
			throw EXCEPTION_ERROR_ACCESSING_VALUE_FILE; /* werfe entsprechende Exception */
			return false; /* und gebe FALSE zurueck */
		}
	} /* for(int i = 0; i < NUM_OF_SIGNALS; i++) */
	return true; /* Schreiben der Ausgaenge korrekt abgelaufen, gebe TRUE zurueck */
}
