/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: outputGPIOsysfs.h
 * Funktion: Header zu outputGPIOsysfs.cpp, public-Ableitung der Klasse outputGPIOsysfs von outputGPIO. Definiert die GPIO-Ansteuerung durch das Linux-sysfs
 * Kommentar: Erste Version
 * Name: Andreas Dolp
 * Datum: 22.04.2014
 * Version: 0.1
 ---------------------------*/

#ifndef OUTPUTGPIOSYSFS_H_
#define OUTPUTGPIOSYSFS_H_

#define MAX_PATH_LENGTH 35
#define DEFAULT_GPIO_PATH "/sys/class/gpio"	/* /sys/class/gpio */
#define DEFAULT_GPIO_EXPORT_FILE "/export"	/* /sys/class/gpio/export */
#define DEFAULT_GPIO_PIN_SUBFOLDER "/gpio"	/* /sys/class/gpio/gpioX */
#define DEFAULT_GPIO_PIN_DIRECTION_FILE "/direction"	/* /sys/class/gpio/gpioX/direction */
#define DEFAULT_GPIO_PIN_VALUE_FILE "/value"	/* /sys/class/gpio/gpioX/value */
#define DEFAULT_GPIO_PIN_DIRECTION_OUT "out"

#define EXCEPTION_ERROR_ACCESSING_EXPORT_FILE 1
#define EXCEPTION_ERROR_ACCESSING_DIRECTION_FILE 2
#define EXCEPTION_ERROR_ACCESSING_VALUE_FILE 3

#include "outputGPIO.h"

class outputGPIOsysfs: public outputGPIO {
public:
	outputGPIOsysfs(const unsigned int iaAdrGPIOPinsToSet[NUM_OF_SIGNALS]); /* Konstruktor, Reihenfolge: XF,XB,YF,YB,ZF,ZB,USBErr */
	bool write();	/* Funktion zum Schreiben der Werte an die GPIO-Ausgaenge */
	bool init();	/* Funktion zum Initialisieren der GPIO-Ausgaenge */
private:
	unsigned int iaAdrGPIOPins[NUM_OF_SIGNALS];	/* Adresse / Nummern der GPIO-Pins (bzgl. Hardware-Pin-Map)*/
};

#endif /* OUTPUTGPIOSYSFS_H_ */
