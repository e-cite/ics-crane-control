/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: inputMovement.cpp
 * Funktion: Implementierung der Klasse inputMovement, Programmierung der Methoden
 * Kommentar: Ueberarbeitungen, erste vollstaendig lauffaehige Version
 * Name: Andreas Dolp
 * Datum: 16.05.2014
 * Version: 1.0
 ---------------------------*/

#include "inputMovement.h"
#include <cstring> /* strcpy */

/*
 * @brief Konstruktor
 * @param cpDevicePathToSet Dateipfad des dem Objekt zugeordneten Devices, terminiert mit \0
 *
 * setzt Dateipfad des dem Objekt zugeordneten Devices und initialisiert die Variablen
 */
inputMovement::inputMovement(const char* cpDevicePathToSet) {
	strcpy(this->cpDevicePath,cpDevicePathToSet);
	this->bBtn1 = false;
	this->bBtn2 = false;
	this->bBtn3 = false;
	this->iDX = 0;
	this->iDY = 0;
}

/*
 * @brief Getter-Methode
 * @return inputMovement::bBtn1
 */
bool inputMovement::getBtn1() {
	return this->bBtn1;
}

/*
 * @brief Getter-Methode
 * @return inputMovement::bBtn2
 */
bool inputMovement::getBtn2() {
	return this->bBtn2;
}

/*
 * @brief Getter-Methode
 * @return inputMovement::bBtn3
 */
bool inputMovement::getBtn3() {
	return this->bBtn3;
}

/*
 * @brief Getter-Methode
 * @return inputMovement::iDX
 */
int inputMovement::getDX() {
	return this->iDX;
}

/*
 * @brief Getter-Methode
 * @return inputMovement::iDY
 */
int inputMovement::getDY() {
	return this->iDY;
}
