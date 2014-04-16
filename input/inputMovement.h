/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: inputMovement.h
 * Funktion: Header zu inputMovement.cpp, Definition der Klasse inputMovement, welche die grundlegenden Eigenschaften einer Eingabe definiert
 * Kommentar: Erste Version
 * Name: Andreas Dolp
 * Datum: 08.04.2014
 * Version: 0.1
 ---------------------------*/

#ifndef INPUTMOVEMENT_H_
#define INPUTMOVEMENT_H_

class inputMovement {
public:
	inputMovement();			/* Konstruktor */
	virtual ~inputMovement();	/* virtueller Destruktor */
	virtual bool read() = 0;	/* Funktion zum Auslesen des jeweiligen Devices */
	bool getClickLeft();		/* Getter-Funktion liefert Zustand des Linksklicks */
	bool getClickRight();		/* Getter-Funktion liefert Zustand des Rechtsklicks */
	bool getClickMiddle();		/* Getter-Funktion liefert Zustand des Mittelklicks */
	int getDX();				/* Getter-Funktion liefert Wert der Mausbewegung in X-Richtung */
	int getDY();				/* Getter-Funktion liefert Wert der Mausbewegung in Y-Richtung */
private:
	bool bClickLeft;			/* Linksklick */
	bool bClickRight;			/* Rechtsklick */
	bool bClickMiddle;			/* Mittelklick */
	int iDX;					/* Bewegung in X-Richtung */
	int iDY;					/* Bewegung in Y-Richtung */
};

#endif /* INPUTMOVEMENT_H_ */
