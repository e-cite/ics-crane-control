/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: outputGPIOsysfs.h
 * Funktion: Header zu outputGPIOsysfs.cpp, Definition der Klasse outputGPIOsysfs
 * Kommentar: Ueberarbeitungen, erste vollstaendig lauffaehige Version
 * Name: Andreas Dolp
 * Datum: 08.05.2014
 * Version: 1.0
 ---------------------------*/

#ifndef OUTPUTGPIOSYSFS_H_
#define OUTPUTGPIOSYSFS_H_

#define MAX_PATH_LENGTH 50

/* Festlegung der Standard-Dateipfade zu den GPIO-Output-Files des SYSFS */
#define DEFAULT_GPIO_PATH "/sys/class/gpio" /* /sys/class/gpio */
#define DEFAULT_GPIO_EXPORT_FILE "/export" /* /sys/class/gpio/export */
#define DEFAULT_GPIO_PIN_SUBFOLDER "/gpio" /* /sys/class/gpio/gpioX */
#define DEFAULT_GPIO_PIN_DIRECTION_FILE "/direction" /* /sys/class/gpio/gpioX/direction */
#define DEFAULT_GPIO_PIN_VALUE_FILE "/value" /* /sys/class/gpio/gpioX/value */
#define DEFAULT_GPIO_PIN_DIRECTION_OUT "out"

#define EXCEPTION_ERROR_ACCESSING_EXPORT_FILE -20
#define EXCEPTION_ERROR_ACCESSING_DIRECTION_FILE -21
#define EXCEPTION_ERROR_ACCESSING_VALUE_FILE -22

#include "outputGPIO.h"

/*
 * Definition der Klasse outputGPIOsysfs als public-Ableitung von outputGPIO
 * Definiert die GPIO-Ansteuerung durch das Linux-SYSFS geeignet fuer die Kran-Signale
 */
class outputGPIOsysfs: public outputGPIO {
public:
	outputGPIOsysfs(const unsigned int*); /* Konstruktor */
	~outputGPIOsysfs(); /* Destruktor */
	bool init(); /* Methode zum Initialisieren der GPIO-Ausgaenge */
	bool write(); /* Methode zum Schreiben der gesetzten Signale an die GPIO-Ausgaenge */

private:
	unsigned int iaGPIOPinsAddress[NUM_OF_SIGNALS];	/* Adresse / Nummern der GPIO-Pins (bzgl. Hardware-Pin-Map, nicht PIN-Nummer) */
};

#endif /* OUTPUTGPIOSYSFS_H_ */
