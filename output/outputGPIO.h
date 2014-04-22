/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: outputGPIO.h
 * Funktion: Header zu outputGPIO.cpp, Definition der Klasse outoutGPIO, welche die grundlegenden Eigenschaften einer Kran-Ausgabe definiert
 * Kommentar: Erste Version
 * Name: Andreas Dolp
 * Datum: 22.04.2014
 * Version: 0.1
 ---------------------------*/

#ifndef OUTPUTGPIO_H_
#define OUTPUTGPIO_H_

#define GPIO_AXES_SIGNALS_ACTIVE 1	/* low-aktiv = 0, high-aktiv = 1 */
#define GPIO_USB_ERROR_ACTIVE 0		/* low-aktiv = 0, high-aktiv = 1 */
#define NUM_OF_SIGNALS 7			/* Anzahl der verwendeten Signale, fuer Array benoetigt */

#define EXCEPTION_NO_VALID_SIGNAL_TO_GET 1
#define EXCEPTION_INCONSISTENT_SIGNALS_TO_SET 2

class outputGPIO {
public:
	outputGPIO();				/* Konstruktor */
	virtual ~outputGPIO() {};	/* virtueller Destruktor */
	virtual bool init() = 0;	/* virtuelle Methode init zum Initialisieren der Ausgaenge */
	virtual bool write() = 0;	/* Virtuelle Methode write zum Schreiben der Werte an die Ausgaenge*/
	bool setSignals(bool baGPIOSignalsToSet[NUM_OF_SIGNALS]);	/* Setter-Funktion zum Setzen der Achs-Signale, prueft auf Konsistenz */
	bool getSignal(int iNumOfGPIOSignal);	/* Getter-Funktion gibt den Zustand des im Parameter uebergebenen Signals zurueck */

private:
	bool baGPIOSignals[NUM_OF_SIGNALS];	/* Array der Signalzustaende, Reihenfolge: XF,XB,YF,YB,ZF,ZB,USBErr */
};

#endif /* OUTPUTGPIO_H_ */
