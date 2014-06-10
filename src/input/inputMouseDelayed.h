/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: inputMouseDelayed.h
 * Funktion: Header zu inputMouseDelayed.cpp, Deklaration der Klasse inputMouseDelayed
 * Kommentar: Erste Version
 * Name: Andreas Dolp
 * Datum: 28.05.2014
 * Version: 0.1
 ---------------------------*/

#ifndef INPUTMOUSEDELAYED_H_
#define INPUTMOUSEDELAYED_H_

#define EXCEPTION_READ_MOUSE_DELAYED_ERROR -13

#include "inputMouse.h"
#include <sys/time.h> /* struct timeval */

/*
 * Deklaration der Klasse inputMouseDelayed als public-Ableitung von inputMouse
 * Implementiert die Methoden zum Einlesen einer USB-Maus, jedoch werden alle Klicks um eine variable Zeit verzoegert
 */
class inputMouseDelayed: public inputMouse {
public:
	inputMouseDelayed(const char*); /* Konstruktor */
	~inputMouseDelayed(); /* Destruktor */
	bool read(); /* Methode zum Auslesen eines USB-Maus-Devices, jedoch werden alle Klicks um eine variable Zeit verzoegert */
	void setTimevalKlickDelay(unsigned int, unsigned int = 0); /* Setter-Methode zum Setzen der Verzoegerung */
private:
	struct timeval timevalLastBtn1HighEvent; /* struct timeval, Zeitpunkt zu dem letztes Value 1 Event des Btn1 eingegangen ist */
	struct timeval timevalLastBtn2HighEvent; /* struct timeval, Zeitpunkt zu dem letztes Value 1 Event des Btn2 eingegangen ist */
	struct timeval timevalLastBtn3HighEvent; /* struct timeval, Zeitpunkt zu dem letztes Value 1 Event des Btn3 eingegangen ist */
	struct timeval timevalDelay; /* struct timeval, Verzoegerungsdauer */
};

#endif /* INPUTMOUSEDELAYED_H_ */
