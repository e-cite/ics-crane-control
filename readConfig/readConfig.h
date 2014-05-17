/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: readConfig.h
 * Funktion: Header zu readConfig.cpp, Deklariert die struct configValues
 * Kommentar: Anpassungen an aktuellen Projektstand, Einbau der benoetigten Werte
 * Name: Andreas Dolp
 * Datum: 17.05.2014
 * Version: 1.0
 ---------------------------*/

#ifndef READCONFIG_H_
#define READCONFIG_H_

#define SIZE_OF_PATH_LENGTH 100

#define EXCEPTION_CPVALUETOFIND_NOT_FOUND 30
#define EXCEPTION_UNABLE_TO_OPEN_FILE -31

/*
 * @brief Deklariere struct configValues
 *
 * Diese beinhaltet alle aus der config.ini eingelesenen Werte
 */
typedef struct configValues {
	 char caDevicePath[SIZE_OF_PATH_LENGTH];
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
	 bool bFlagDebug;
	 bool bFlagNcurses;
} configValues;

bool readConfigSingleValue(const char* cpConfigFilePath, const char* cpValueToFind, void* vpResult);
configValues* readConfig(const char* cpConfigFilePath);

#endif /* READCONFIG_H_ */
