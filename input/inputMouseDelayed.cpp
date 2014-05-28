/*---------------------------
 * Projekt: ICS - Kran Neubau
 * Dateiname: inputMouseDelayed.cpp
 * Funktion: Implementierung der Klasse inputMouseDelayed, Programmierung der Methoden
 * Kommentar: Erste Version
 * Name: Andreas Dolp
 * Datum: 28.05.2014
 * Version: 0.1
 ---------------------------*/

#include "inputMouseDelayed.h"
#include <fcntl.h> /* open */
#include <linux/input.h> /* ioctl */
#include <unistd.h> /* read, close */

/*
 * @brief Konstruktor
 * @param cpMousePathToSet Dateipfad des dem Objekt zugeordneten Maus-Devices, terminiert mit \0
 * @throw EXCEPTION_READ_MOUSE_DELAYED_ERROR Fehler beim Oeffnen des Maus-Devices und wenn Vendor & Product-ID nicht zutreffend
 *
 * setzt Dateipfad des dem Objekt zugeordneten Devices und oeffnet dieses read-only
 */
inputMouseDelayed::inputMouseDelayed(const char* cpMousePathToSet)
	: inputMouse(cpMousePathToSet) { /* Weitergabe von cpMousePathToSet an Konstruktor der inputMouse-Klasse */

	timerclear(&this->timevalDelay); /* Initialisiere alle struct timeval mit 0 */
	timerclear(&this->timevalLastBtn1HighEvent);
	timerclear(&this->timevalLastBtn2HighEvent);
	timerclear(&this->timevalLastBtn3HighEvent);

	this->fds.events = POLLIN; /* Initialisiere fds.events mit POLLIN Bitmaske. POLLIN = pruefe ob einkommende Daten vorhanden */
	if ((this->fds.fd = open(this->cpDevicePath, O_RDONLY)) <= 0) { /* Oeffne Maus-Device-File read-only */
		throw EXCEPTION_READ_MOUSE_DELAYED_ERROR; /* Wenn Oeffnen nicht moeglich, werfe entsprechende Exception */
		this->fds.fd = 0; /* und setze Dateizeiger = 0 */
	}

	unsigned short id[4]; /* Array fuer die InputDevice-IDs */
	ioctl(this->fds.fd,EVIOCGID,id); /* Lese InputDevice-IDs in id-Array ein */

	/* Objekte dieser Art nur bei folgenden Eingabegeraeten zulassen */
	if(id[ID_VENDOR] == 0x178a && id[ID_PRODUCT] == 0x2)
		return;
	if(id[ID_VENDOR] == 0xe85 && id[ID_PRODUCT] == 0xf)
		return;

	/* Wenn nicht eines der oben definierten Geraete */
	throw EXCEPTION_READ_MOUSE_DELAYED_ERROR; /* werfe entsprechende Exception */
	this->fds.fd = 0; /* und setze Dateizeiger = 0 */
}

/*
 * @brief Destruktor
 *
 * schliesst Dateizeiger
 */
inputMouseDelayed::~inputMouseDelayed() {
	close(this->fds.fd);
}

/*
 * @brief Setter-Methode zum Setzen der Verzoegerung
 * @param iSecondsToSet Verzoegerungszeit in Sekunden
 * @param iMicrosecondsToSet Verzoegerungszeit in Microsekunden (muss kleiner als 1 000 000 sein!)
 */
void inputMouseDelayed::setTimevalKlickDelay(unsigned int iSecondsToSet, unsigned int iMicrosecondsToSet) {
	if (iMicrosecondsToSet < 1000000) /* Pruefe ob Wert der Microsekunden kleiner als 1 Sekunde */
		this->timevalDelay.tv_usec = iMicrosecondsToSet; /* Wenn gueltig, schreibe in Objekt */
	else
		this->timevalDelay.tv_usec = iMicrosecondsToSet % 1000000; /* Wenn nicht gueltig, wende Modulo-Operation an und schreibe nur Rest */
	this->timevalDelay.tv_sec = iSecondsToSet; /* schreibe Sekunden in Objekt */
}

/*
 * @brief Methode zum Auslesen eines USB-Maus-Devices, jedoch werden alle Klicks um eine variable Zeit verzoegert
 * @brief Methode sonst identisch mit inputMouse::read()
 *
 * @return TRUE bei erfolgreichem Auslesen, FALSE bei Fehler
 */
bool inputMouseDelayed::read() {
	int iPollReturnValue = -1; /* Variable zum Speichern des Rueckgabewerts der poll()-Funktion */
	struct input_event ie; /* input_event struct des aktuellen Lesevorgangs */
	struct timeval timevalBuffer; /* struct timeval, dient als Zwischenvariable */


	if (this->fds.fd > 0) { /* Wenn Datei in Konstruktor korrekt geoeffnet */
		iPollReturnValue = poll(&this->fds,1,POLLING_MOUSE_TIMEOUT_MS); /* Aufruf der poll()-Funktion mit Adresse der pollfd-struct, lese 1 struct und Timeout in ms */

		if (iPollReturnValue > 0) { /* Wenn polling erfolgreich, d.h. Daten anstehend */

			if (::read(this->fds.fd, &ie, sizeof(struct input_event)) <= 0) { /* Lese Inhalt des Dateizeigers in die input_event struct ie */
				throw EXCEPTION_READ_MOUSE_ERROR; /* Wenn Lesevorgang fehlschlaegt, werfe entsprechende Exception */
				return false; /* und gebe FALSE zurueck */
			}

			if (ie.type == EV_MSC) /* Da beim Einlesen von Klicks zwei Events auftreten, lese bei ie.type = EV_MSC(=4) nochmals */
				if (::read(this->fds.fd, &ie, sizeof(struct input_event)) <= 0) {
					throw EXCEPTION_READ_MOUSE_ERROR;
					return false;
				}

			/* Korrekt eingelesene Daten */

			/* Pruefe auf Linksklick */
			if (ie.type == EV_KEY && ie.code == BTN_LEFT) {
				this->bBtn1 = ie.value; /* und schreibe Wer7t in eigenes Objekt */
				if (ie.value == 1 ) {
					this->timevalLastBtn1HighEvent = ie.time; /* Speichere Zeitpunkt zu dem letztes Value 1 Event des Btn1 eingegangen ist */
					timerclear(&this->timevalLastBtn2HighEvent); /* und setze Zeitpunkte der anderen Events zurueck */
					timerclear(&this->timevalLastBtn3HighEvent);
				}
			}
			/* Pruefe auf Rechtsklick */
			if (ie.type == EV_KEY && ie.code == BTN_RIGHT) {
				this->bBtn2 = ie.value; /* und schreibe Wert in eigenes Objekt */
				if (ie.value == 1 ) {
					this->timevalLastBtn2HighEvent = ie.time; /* Speichere Zeitpunkt zu dem letztes Value 1 Event des Btn2 eingegangen ist */
					timerclear(&this->timevalLastBtn1HighEvent); /* und setze Zeitpunkte der anderen Events zurueck */
					timerclear(&this->timevalLastBtn3HighEvent);
				}
			}
			/* Pruefe auf Mittelklick */
			if (ie.type == EV_KEY && ie.code == BTN_MIDDLE) {
				this->bBtn3 = ie.value; /* und schreibe Wert in eigenes Objekt */
				if (ie.value == 1 ) {
					this->timevalLastBtn3HighEvent = ie.time; /* Speichere Zeitpunkt zu dem letztes Value 1 Event des Btn3 eingegangen ist */
					timerclear(&this->timevalLastBtn1HighEvent); /* und setze Zeitpunkte der anderen Events zurueck */
					timerclear(&this->timevalLastBtn2HighEvent);
				}
			}
			/* Pruefe auf X-Verschiebung */
			if (ie.type == EV_REL && ie.code == REL_X)
				this->iDX = ie.value; /* und schreibe Wert in eigenes Objekt */
			/* Pruefe auf Y-Verschiebung */
			if (ie.type == EV_REL && ie.code == REL_Y)
				this->iDY = ie.value; /* und schreibe Wert in eigenes Objekt */

		} /* if (iPollRetVal > 0) */

		if (iPollReturnValue == 0) { /* Wenn poll() mit Timeout beendet wird */
			this->iDX = 0; /* Setze X-Verschiebung = 0 */
			this->iDY = 0; /* Setze Y-Verschiebung = 0 */
		}

		if (iPollReturnValue < 0) { /* Wenn poll() mit Error beendet wird */
			throw EXCEPTION_POLLING_ERROR; /* werfe entsprechende Exception */
			return false; /* und gebe FALSE zurueck */
		}

		gettimeofday(&timevalBuffer,0); /* Lese aktuelle Uhrzeit in timevalBuffer */
		timersub(&timevalBuffer,&this->timevalDelay,&timevalBuffer); /* Subtrahiere Verzoegerung von aktueller Zeit */

		if ( timercmp(&this->timevalLastBtn1HighEvent,&timevalBuffer,>) ) /* Wenn letztes Event weniger lang als Verzoegerungszeit her ist */
			this->bBtn1 = 1; /* setze bBtn1 auf HIGH */
		else /* wenn nicht */
			this->bBtn1 = 0; /* setze bBtn1 auf FALSE */

		if ( timercmp(&this->timevalLastBtn2HighEvent,&timevalBuffer,>) ) /* Wenn letztes Event weniger lang als Verzoegerungszeit her ist */
			this->bBtn2 = 1; /* setze bBtn2 auf HIGH */
		else /* wenn nicht */
			this->bBtn2 = 0; /* setze bBtn2 auf FALSE */

		if ( timercmp(&this->timevalLastBtn3HighEvent,&timevalBuffer,>) ) /* Wenn letztes Event weniger lang als Verzoegerungszeit her ist */
			this->bBtn3 = 1; /* setze bBtn3 auf HIGH */
		else /* wenn nicht */
			this->bBtn3 = 0; /* setze bBtn3 auf FALSE */

		return true; /* da erfolgreich, gebe TRUE zurueck */

	} /* if (this->fds.fd > 0) Wenn Datei in Konstruktor korrekt geoeffnet */

	return false; /* In allen anderen Faellen gebe FALSE zurueck*/
} /* bool inputMouse::read() */
