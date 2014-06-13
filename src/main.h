/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: main.h
 * Funktion: Header zu main.cpp
 * Kommentar: Anpassungen in der Versionsnummerierung
 * Name: Andreas Dolp
 * Datum: 13.06.2014
 * Version: 1.5
 ---------------------------*/

#ifndef MAIN_H_
#define MAIN_H_

#ifdef NCURSES
#define DEBUG
#endif

#define VERSION "1.5_final"

#define DEFAULT_DELTA_MIN 10
#define DEFAULT_DELAY_SECONDS 0
#define DEFAULT_DELAY_MICROSECONDS 500000

int main ( int argc, char* argv[] );

#endif /* MAIN_H_ */
