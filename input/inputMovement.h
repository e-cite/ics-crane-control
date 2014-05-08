/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: inputMovement.h
 * Funktion: Header zu inputMovement.cpp, Definition der Klasse inputMovement
 * Kommentar: Ueberarbeitungen, erste vollstaendig lauffaehige Version
 * Name: Andreas Dolp
 * Datum: 08.05.2014
 * Version: 1.0
 ---------------------------*/

#ifndef INPUTMOVEMENT_H_
#define INPUTMOVEMENT_H_

#define MAX_PATH_LENGTH 100

/*
 * Definition der Klasse inputMovement
 * Definiert die grundlegenden Eigenschaften eines Eingabe-Devices
 */
class inputMovement {
public:
	inputMovement(const char*); /* Konstruktor */
	virtual ~inputMovement() {}; /* virtueller Destruktor (zur Unterdrueckung einer Compiler-Warnung) */
	virtual bool read() = 0; /* Virtuelle, abstrakte Funktion zum Auslesen des jeweiligen Devices */
	bool getBtn1(); /* Getter-Funktion liefert Zustand des Tasters 1 */
	bool getBtn2(); /* Getter-Funktion liefert Zustand des Tasters 2 */
	bool getBtn3(); /* Getter-Funktion liefert Zustand des Tasters 3 */
	int getDX(); /* Getter-Funktion liefert eingelesenen Wert der Bewegung in X-Richtung */
	int getDY(); /* Getter-Funktion liefert eingelesenen Wert der Bewegung in Y-Richtung */
protected:
	char cpDevicePath[MAX_PATH_LENGTH];	/* Dateipfad des dem Objekt zugeordneten Devices */
	bool bBtn1; /* Eingelesener Wert der Taste 1 */
	bool bBtn2; /* Eingelesener Wert der Taste 2 */
	bool bBtn3; /* Eingelesener Wert der Taste 3 */
	int iDX; /* Eingelesener Wert der Bewegung in X-Richtung */
	int iDY; /* Eingelesener Wert der Bewegung in Y-Richtung */
};

#endif /* INPUTMOVEMENT_H_ */
