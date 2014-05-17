/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: readConfig.h
 * Funktion: Header zu readConfig.cpp, Deklariert die struct configValues
 * Kommentar: Anpassungen im Kommentar, Exception-Codes
 * Name: Andreas Dolp
 * Datum: 17.05.2014
 * Version: 0.3
 ---------------------------*/

#ifndef READCONFIG_H_
#define READCONFIG_H_

#define SIZE_OF_STRING_LENGTH 100

#define EXCEPTION_CPVALUETOFIND_NOT_FOUND 30
#define EXCEPTION_UNABLE_TO_OPEN_FILE -31

/*
 * @brief Deklariere struct configValues
 *
 * Diese beinhaltet alle aus der config.ini eingelesenen Werte
 */
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

bool readConfigSingleValue(const char* cpConfigFilePath, const char* cpValueToFind, void* vpResult);
configValues* readConfig(const char* cpConfigFilePath);

#endif /* READCONFIG_H_ */
