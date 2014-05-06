/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: inputMovement.cpp
 * Funktion: Implementierung der Klasse inputMovement, Programmierung der Funktionen
 * Kommentar: Anpassung und Verallgemeinerung
 * Name: Andreas Dolp
 * Datum: 06.05.2014
 * Version: 0.2
 ---------------------------*/

#include "inputMovement.h"
#include <cstring>	/* strcpy */

/* Konstruktor */
inputMovement::inputMovement(const char* cpPathToSet) {
	/* Vorbelegung der Elemente mit Initialwerten */
	strcpy(this->cpPath,cpPathToSet);
	this->bBtn1 = false;
	this->bBtn2 = false;
	this->bBtn3 = false;
	this->iDX = 0;
	this->iDY = 0;
}

/* Getter-Funktionen */
bool inputMovement::getBtn1() {
	return this->bBtn1;
}
bool inputMovement::getBtn2() {
	return this->bBtn2;
}

bool inputMovement::getBtn3() {
	return this->bBtn3;
}

int inputMovement::getDX() {
	return this->iDX;
}

int inputMovement::getDY() {
	return this->iDY;
}
