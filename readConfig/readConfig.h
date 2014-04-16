/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: readConfig.h
 * Funktion: Header zu readConfig.cpp, Definiert die struct configValues
 * Kommentar: Erste Version
 * Name: Andreas Dolp
 * Datum: 07.04.2014
 * Version: 0.1
 ---------------------------*/

#ifndef READCONFIG_H_
#define READCONFIG_H_

#define SIZE_OF_STRING_LENGTH 100
#define EXCEPTION_CPVALUETOFIND_NOT_FOUND 1
#define EXCEPTION_UNABLE_TO_OPEN_FILE 2

/* Deklariere struct configValues */
/* Diese beinhaltet alle aus der config.ini eingelesenen Werte */
typedef struct configValues {
	 char mousePath[SIZE_OF_STRING_LENGTH];
	 char joystickPath[SIZE_OF_STRING_LENGTH];
	 int gpioXF;
	 int gpioXB;
	 int gpioYF;
	 int gpioYB;
	 int gpioZF;
	 int gpioZB;
	 int gpioUSBError;
} configValues;

/* Liest einen Wert aus der config-Datei in den bereitgestellten Speicherplatz result ein */
bool readConfigSingleValue(char* cpConfigFilePath, char* cpValueToFind, void* vpResult);
/* Legt die im Header definierte struct configValues an */
/* und liest die in dieser Funktion spezifizierte Werte aus der config-File in diese ein */
configValues* readConfig(char* cpConfigFilePath);

#endif /* READCONFIG_H_ */
