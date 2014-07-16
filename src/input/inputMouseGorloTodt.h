/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: inputMouseGorloTodt.h
 * Funktion: Header zu inputMouseGorloTodt.cpp, Deklaration der Klasse inputMouseGorloTodt
 * Kommentar: Erste Version
 * Name: Andreas Dolp
 * Datum: 16.07.2014
 * Version: 0.1
 ---------------------------*/

#ifndef INPUTMOUSEGORLOTODT_H_
#define INPUTMOUSEGORLOTODT_H_

#define EXCEPTION_READ_MOUSE_GORLOTODT_ERROR -14

#include "inputMouse.h"

/*
 * Deklaration der Klasse inputMouseGorloTodt als public-Ableitung von inputMouse
 * Implementiert die Methoden zum Einlesen einer USB-Maus, jedoch spezielle Schwellsetzung
 */
class inputMouseGorloTodt: public inputMouse {
public:
	inputMouseGorloTodt(const char* cpMousePathToSet); /* Konstruktor */
	~inputMouseGorloTodt(); /* Destruktor */
};

#endif /* INPUTMOUSEGORLOTODT_H_ */
