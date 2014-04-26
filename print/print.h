/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: print.h
 * Funktion: Header zu print.cpp
 * Kommentar: Erste Version
 * Name: Andreas Dolp
 * Datum: 26.04.2014
 * Version: 0.1
 ---------------------------*/

#ifndef PRINT_H_
#define PRINT_H_
#include "../main.h"	/* _DEBUG; VERSION */
#include "../output/outputGPIOsysfs.h"	/* NUM_OF_SIGNALS */

void print(const bool baSignals [NUM_OF_SIGNALS], const bool printTitle);
void printTitle(const bool baSignals [NUM_OF_SIGNALS]);
void printError(const char* cpOptErrString, int iOptErrCode);
void printSignals(const bool baSignals [NUM_OF_SIGNALS]);


#endif /* PRINT_H_ */
