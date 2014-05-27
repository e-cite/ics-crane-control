/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: main.h
 * Funktion: Header zu main.cpp
 * Kommentar: Anpassungen an veraenderte print-Ausgabe
 * Name: Andreas Dolp
 * Datum: 27.05.2014
 * Version: 1.3
 ---------------------------*/

#ifndef MAIN_H_
#define MAIN_H_

#ifdef NCURSES
#define DEBUG
#endif

#define VERSION 1.3

#define DEFAULT_DELTA_MIN 10

int main ( int argc, char* argv[] );

#endif /* MAIN_H_ */
