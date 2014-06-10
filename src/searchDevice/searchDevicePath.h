/*---------------------------
 * Projekt: searchDevicePath
 * Dateiname: searchDevicePath.h
 * Funktion: Header-Datei zu searchDevicePath.cpp
 * Kommentar: Erste Version, funktionsfaehig
 * Name: Andreas Dolp
 * Datum: 15.05.2014
 * Version: 1.0
 ---------------------------*/

#ifndef SEARCHDEVICEPATH_H_
#define SEARCHDEVICEPATH_H_

#include <string> /* std::string */

#define ALWAYS_CONTAINED_STRING "event" /* String welcher ebenfalls zusaetzlich enthalten sein muss */

std::string searchDevicePath(std::string, std::string); /* Prototyp */

#endif /* SEARCHDEVICEPATH_H_ */
