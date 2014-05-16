/*---------------------------
 * Projekt: searchDevicePath
 * Dateiname: searchDevicePath.cpp
 * Funktion: Einlesen des Inhalts eines Dateiverzeichnisses, in Dateinamen nach String suchen, der zusaetzlich noch das Wort ALWAYS_CONTAINED_STRING enthaelt
 * Kommentar: Erste Version, funktionsfaehig
 * Name: Andreas Dolp
 * Datum: 16.05.2014
 * Version: 1.0
 ---------------------------*/

#include "searchDevicePath.h"
#include <string> /* std::string */
#include <dirent.h> /* DIR* */
#include <fstream> /* open */

/*
 * @brief Funktion zum Suchen zweier Strings in den Dateinamen eines Ordners
 * @param strFilePathToSearchIn Gibt Pfad des Ordners an, in welchem gesucht werden soll
 * @param strStringToFindInDirectory Gibt den String an, welcher in den Dateinamen gesucht werden soll
 * @return Vollstaendiger Dateiname inkl. Pfad falls erfolgreich gefunden, sonst std::string()
 */

std::string searchDevicePath(std::string strFilePathToSearchIn, std::string strStringToFindInDirectory) {
	DIR* dirpDirectory = NULL; /* Zeiger auf Dateiverzeichnis */
	struct dirent* structdirentpEntries = NULL; /* Zeiger auf einen Dateiverzeichniseintrag */
	std::string strBuffer = std::string(); /* String-Buffer */

	dirpDirectory = opendir(strFilePathToSearchIn.c_str()); /* Oeffne Dateiverzeichnis */

	if(dirpDirectory) { /* Wenn Dateiverzeichnis korrekt geoeffnet */
		while( (structdirentpEntries = readdir( dirpDirectory )) != NULL ) { /* while-Schleife, solange wie Lesen eines Dateiverzeichniseintrags erfolgreich */
			strBuffer = structdirentpEntries->d_name; /* Weise Dateinamen des gelesenen Dateiverzeichniseintrags strBuffer zu */
			if( strBuffer.find(strStringToFindInDirectory) != std::string::npos) /* Suche strStringToFindInDirectory in Dateinamen */
				if( strBuffer.find(ALWAYS_CONTAINED_STRING) != std::string::npos) { /* Pruefe ob auch noch ALWAYS_CONTAINED_STRING enthalten ist */
					closedir(dirpDirectory); /* Schliesse Dateizeiger */
					if (strFilePathToSearchIn.at(strFilePathToSearchIn.length()-1) == '/' ) /* pruefe ob uebergebener Pfad mit / endet */
						return (strFilePathToSearchIn + strBuffer); /* Wenn ja, gebe Pfad + Inhalt des Buffers zurueck */
					else
						return (strFilePathToSearchIn + '/' + strBuffer); /* Wenn nein, gebe Pfad + / + Inhalt des Buffers zurueck */
				} /* if */
		} /* while */
	} /* if(dirpDirectory) */

	return std::string(); /* Im Fehlerfall, gebe leeren String zurueck */
}
