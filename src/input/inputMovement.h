/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: inputMovement.h
 * Funktion: Header zu inputMovement.cpp, Deklaration der Klasse inputMovement
 * Kommentar: Fehlerverbesserungen
 * Name: Andreas Dolp
 * Datum: 16.05.2014
 * Version: 1.1
 ---------------------------*/

#ifndef INPUTMOVEMENT_H_
#define INPUTMOVEMENT_H_

#define MAX_PATH_LENGTH_INPUTMOVEMENT 100

/*
 * Deklaration der Klasse inputMovement
 * Implementiert die grundlegenden Eigenschaften eines Eingabe-Devices
 */
class inputMovement {
public:
	inputMovement(const char*); /* Konstruktor */
	virtual ~inputMovement() {}; /* virtueller Destruktor (zur Unterdrueckung einer Compiler-Warnung) */
	virtual bool read() = 0; /* virtuelle, abstrakte Methode zum Auslesen des jeweiligen Devices */
	bool getBtn1(); /* Getter-Methode liefert Zustand des Tasters 1 */
	bool getBtn2(); /* Getter-Methode liefert Zustand des Tasters 2 */
	bool getBtn3(); /* Getter-Methode liefert Zustand des Tasters 3 */
	int getDX(); /* Getter-Methode liefert eingelesenen Wert der Bewegung in X-Richtung */
	int getDY(); /* Getter-Methode liefert eingelesenen Wert der Bewegung in Y-Richtung */

protected:
	char cpDevicePath[MAX_PATH_LENGTH_INPUTMOVEMENT];	/* Dateipfad des dem Objekt zugeordneten Devices */
	bool bBtn1; /* Eingelesener Wert der Taste 1 */
	bool bBtn2; /* Eingelesener Wert der Taste 2 */
	bool bBtn3; /* Eingelesener Wert der Taste 3 */
	int iDX; /* Eingelesener Wert der Bewegung in X-Richtung */
	int iDY; /* Eingelesener Wert der Bewegung in Y-Richtung */
};

#endif /* INPUTMOVEMENT_H_ */
