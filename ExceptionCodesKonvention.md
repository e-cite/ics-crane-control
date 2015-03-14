# Konvention der Exception-Codes #

  * Exception-Codes > 0: Unkritische Fehler
  * Exception-Codes < 0: Kritische Fehler


## input-Klasse ##
Exception-Code: 1X

  * -10: EXCEPTION\_POLLING\_ERROR
  * -11: EXCEPTION\_READ\_MOUSE\_ERROR
  * -12: EXCEPTION\_READ\_JOYSTICK\_ERROR
  * -13: EXCEPTION\_READ\_MOUSE\_DELAYED\_ERROR
  * -14: EXCEPTION\_READ\_MOUSE\_GORLOTODT\_ERROR

## output-Klasse ##
Exception-Code: 2X

  * 20: EXCEPTION\_PARAMETER\_OUT\_OF\_RANGE
  * -20: EXCEPTION\_ERROR\_ACCESSING\_EXPORT\_FILE
  * -21: EXCEPTION\_ERROR\_ACCESSING\_DIRECTION\_FILE
  * -22: EXCEPTION\_ERROR\_ACCESSING\_VALUE\_FILE

## readConfig-Funktion ##
Exception-Code: 3X

  * 30 = EXCEPTION\_CPVALUETOFIND\_NOT\_FOUND + Reihenfolge-Nummer des nicht gefundenen Paramters
  * -30: EXCEPTION\_READ\_FILE\_ERROR