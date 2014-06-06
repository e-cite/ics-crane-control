/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: outputGPIO.h
 * Funktion: Header zu outputGPIO.cpp, Deklaration der Klasse outputGPIO
 * Kommentar: Ueberarbeitungen, erste vollstaendig lauffaehige Version
 * Name: Andreas Dolp
 * Datum: 16.05.2014
 * Version: 1.1
 ---------------------------*/

#ifndef OUTPUTGPIO_H_
#define OUTPUTGPIO_H_

#define GPIO_SIGNAL_ACTIVE_STATE 1 /* low-aktiv = 0, high-aktiv = 1 */
#define GPIO_USBERROR_ACTIVE_STATE 0 /* low-aktiv = 0, high-aktiv = 1 */

#define NUM_OF_SIGNALS 7 /* Anzahl der verwendeten Signale */

/* Positionen der Signale im baGPIOSignals-Array */
#define SIGNAL_USBERR 0
#define SIGNAL_XF 1
#define SIGNAL_XB 2
#define SIGNAL_YF 3
#define SIGNAL_YB 4
#define SIGNAL_ZF 5
#define SIGNAL_ZB 6

#define EXCEPTION_PARAMETER_OUT_OF_RANGE 20

/*
 * Deklaration der Klasse outputGPIO
 * Implementiert die grundlegenden Eigenschaften einer GPIO-Ausgabe bezogen auf die Kran-Signale
 */
class outputGPIO {
public:
	outputGPIO(); /* Konstruktor */
	virtual ~outputGPIO() {}; /* virtueller Destruktor (zur Unterdrueckung einer Compiler-Warnung) */
	virtual bool init() = 0; /* virtuelle, abstrakte Methode zum Initialisieren der GPIO-Ausgaenge */
	virtual bool write() = 0; /* virtuelle, abstrakte Methode zum Schreiben der gesetzten Signale an die GPIO-Ausgaenge */
	bool setSignals(const bool*); /* Setter-Methode zum Setzen der Signale */
	void setUSBErrActive(); /* Setter-Methode zum AKTIV Setzen des USB-Errors und zum NICHT AKTIV Setzen der uebrigen Signale */
	bool getSignal(const unsigned int); /* Getter-Methode liefert den Wert des im Parameter uebergebenen Signals */

private:
	bool baGPIOSignals[NUM_OF_SIGNALS];	/* Array der Signalzustaende, Reihenfolge siehe #define-Sektion dieser Datei */
};

#endif /* OUTPUTGPIO_H_ */
