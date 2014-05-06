/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: inputMovement.h
 * Funktion: Header zu inputMovement.cpp, Definition der Klasse inputMovement, welche die grundlegenden Eigenschaften einer Eingabe definiert
 * Kommentar: Anpassung und Verallgemeinerung
 * Name: Andreas Dolp
 * Datum: 06.05.2014
 * Version: 0.2
 ---------------------------*/

#ifndef INPUTMOVEMENT_H_
#define INPUTMOVEMENT_H_

#define SIZE_OF_STRING_LENGTH 100

class inputMovement {
public:
	inputMovement(const char* cpPathToSet);	/* Konstruktor */
	virtual ~inputMovement() {};/* virtueller Destruktor */
	virtual bool read() = 0;	/* Funktion zum Auslesen des jeweiligen Devices */
	bool getBtn1();	/* Getter-Funktion liefert Zustand des Tasters 1 */
	bool getBtn2();	/* Getter-Funktion liefert Zustand des Tasters 2 */
	bool getBtn3();	/* Getter-Funktion liefert Zustand des Tasters 3 */
	int getDX();	/* Getter-Funktion liefert Wert der Bewegung in X-Richtung */
	int getDY();	/* Getter-Funktion liefert Wert der Bewegung in Y-Richtung */
protected:
	char cpPath[SIZE_OF_STRING_LENGTH];	/* Dateipfad des dem Objekt zugeordneten Devices */
	bool bBtn1;	/* Taster 1 */
	bool bBtn2;	/* Taster 2 */
	bool bBtn3;	/* Taster 3 */
	int iDX;	/* Bewegung in X-Richtung */
	int iDY;	/* Bewegung in Y-Richtung */
};

#endif /* INPUTMOVEMENT_H_ */
