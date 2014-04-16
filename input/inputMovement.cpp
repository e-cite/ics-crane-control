/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: inputMovement.cpp
 * Funktion: Implementierung der Klasse inputMovement, Programmierung der Funktionen
 * Kommentar: Erste Version
 * Name: Andreas Dolp
 * Datum: 08.04.2014
 * Version: 0.1
 ---------------------------*/

#include "inputMovement.h"

/* Konstruktor */
inputMovement::inputMovement() {
	/* Vorbelegung der Elemente mit Initialwerten */
	this->bClickLeft = false;
	this->bClickRight = false;
	this->bClickMiddle = false;
	this->iDX = 0;
	this->iDY = 0;
}

/* Getter-Funktionen */
bool inputMovement::getClickLeft() {
	return this->bClickLeft;
}
bool inputMovement::getClickRight() {
	return this->bClickRight;
}

bool inputMovement::getClickMiddle() {
	return this->bClickMiddle;
}

int inputMovement::getDX() {
	return this->iDX;
}

int inputMovement::getDY() {
	return this->iDY;
}
