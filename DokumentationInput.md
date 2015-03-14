# **Dokumentation und Erläuterung der input-Klassen** #

# **inputMovement-Klasse** #
# Dient als Vorlage und definiert die grundlegenden Eigenschaft einer allgemeinen Eingabebewegung mit drei Tasten, zwei Richtungen und Dateipfad
## Dateien ##
  * input/inputMovement.h
  * input/inputMovement.cpp

## Daten ##
| **Datentyp** | **Variablenname** | **Beschreibung** |
|:-------------|:------------------|:-----------------|
| `char *` | `cpDevicePath` | Dateipfad des Objekts, terminiert mit \n |
| `bool` | `bBtn1` | Eingelesener Wert der Taste 1 |
| `bool` | `bBtn2` | Eingelesener Wert der Taste 2 |
| `bool` | `bBtn3` | Eingelesener Wert der Taste 3 |
| `int` | `iDX` | Eingelesener Wert der Bewegung in X-Richtung |
| `int` | `iDY` | Eingelesener Wert der Bewegung in Y-Richtung |

## Methoden ##
| **return-Typ** | **Methodenname** | **Beschreibung** |
|:---------------|:-----------------|:-----------------|
| `void` | `inputMovement(const char *)` | Konstruktor: Initialisiert die Elemente und kopiert den übergebenen Dateipfad in das Objekt |
| `void` | `~inputMovement()` | Virtueller Destruktor: Dieser wird leer definiert und dient nur zur Unterdrückung einer Compiler-Warnung |
| `bool` | `read()` | Virtuelle, abstrakte Methode zum Auslesen des jeweiligen Devices, wird je nach Ableitung definiert |
| `bool` / `int` | get... | Getter-Methoden: Geben Wert des zugehörigen Elements zurück |

## Beschreibung ##
  * Die abstrakte **inputMovement-Klasse** dient nur als Vorlage für die Ableitung weiterer Klassen. Sie definiert die grundlegenden Eigenschaften einer allgemeinen Eingabebewegung. Dazu zählen neben dem Dateipfad die Zustände dreier Tasten sowie die Bewegungsdeltas in X- und Y-Richtung.
  * Die virtuelle, abstrakte **`read`-Methode** dient zum Auslesen des jeweiligen Devices. Sie wird in den Ableitungen weiter definiert und unterscheidet sich je nach Device.
  * Die **Getter-Methoden** liefern den jeweiligen im Objekt gespeicherten Wert.



# **inputMouse-Klasse** #
# Implementiert die Methoden zum Einlesen einer USB-Maus in die in inputMovement deklarierten Elemente
## Dateien ##
  * input/inputMouse.h
  * input/inputMouse.cpp

## Daten ##
| **Datentyp** | **Variablenname** | **Beschreibung** |
|:-------------|:------------------|:-----------------|
| `struct pollfd` | `fd` | Enthält Informationen zur Art der mit `poll` zu überwachenden Events und den Dateizeiger |

## Methoden ##
| **return-Typ** | **Methodenname** | **Beschreibung** |
|:---------------|:-----------------|:-----------------|
| `void` | `inputMouse(const char *)` | Konstruktor: Weitergabe des übergeben Dateipfads an den inputMovement-Konstruktor, Öffnen des Dateizeigers read-only |
| `void` | `~inputMouse()` | Destruktor: Schließt den Dateizeiger des Objekts |
| `bool` | `read()` | Liest Input-Events einer USB-Maus und schreibt diese in das eigene Objekt |

## Beschreibung ##
  * Die inputMouse-Klasse ist eine public-Ableitung der inputMovement-Klasse.
  * Der **Konstruktor** öffnet den übergebenen Dateizeiger read-only.
  * Der **Destruktor** schließt den Dateizeiger
  * Die **`read`-Methode** prüft zuerst mit Hilfe der `poll`-Funktion, ob Daten anstehen. Dabei wird ein Timeout von `POLLING_MOUSE_TIMEOUT_MS` verwendet. Kommen innerhalb dieser Zeit keine Input-Events an, so werden die X- und Y-Deltas gleich 0 gesetzt. Wenn Daten ankommen, so werden diese mit Hilfe der `read`-Funktion in eine `struct input_event` eingelesen und analysiert. Je nach Art des Input-Events wird der entsprechende Wert im eigenen Objekt gespeichert. Um keine Laufzeitschwierigkeiten zu erhalten wird nur ein Lesevorgang pro Aufruf der Methode durchgeführt. _Eine Ausnahme bildet der Erhalt von `EV_MSC`-Events (=4). Diese treten auf einem **Raspberrypi immer** vor den `EV_KEY`-Events auf. Deshalb wird bei Erhalt eines `EV_MSC`-Events zweimal gelesen, um die eigentlich relevanten `EV_KEY`-Events (Klick-Events) nicht zu verlieren._ Bei einem erfolgreichen Lesevorgang wird `TRUE` zurückgeben, ansonsten `FALSE`.




# **inputJoystick-Klasse** #
# Implementiert die Methoden zum Einlesen eines USB-Joysticks in die in inputMovement deklarierten Elemente
## Dateien ##
  * input/inputJoystick.h
  * input/inputJoystick.cpp

## Daten ##
| **Datentyp** | **Variablenname** | **Beschreibung** |
|:-------------|:------------------|:-----------------|
| `int` | `fd` | Dateizeiger |
| `int` | `iAxisXCentralAbsValue` | Absoluter X-Wert der Mittelpunkts-Stellung |
| `int` | `iAxisYCentralAbsValue` | Absoluter Y-Wert der Mittelpunkts-Stellung |

## Methoden ##
| **return-Typ** | **Methodenname** | **Beschreibung** |
|:---------------|:-----------------|:-----------------|
| `void` | `inputJoystick(const char *)` | Konstruktor: Weitergabe des übergeben Dateipfads an den inputMovement-Konstruktor, Öffnen des Dateizeigers read-only |
| `void` | `~inputJoystick()` | Destruktor: Schließt den Dateizeiger des Objekts |
| `bool` | `read()` | Liest Input-Events eines USB-Joysticks und schreibt diese in das eigene Objekt |

## Beschreibung ##
  * Die inputJoystick-Klasse ist eine public-Ableitung der inputMovement-Klasse.
  * Der **Konstruktor** öffnet den übergebenen Dateizeiger read-only. Bei korrektem Öffnen wird eine `struct input_absinfo` eingelesen, welche die Absolutwerte der X- und Y-Achsen beinhaltet. Diese werden halbiert und so die absoluten Mittelpunktskoordinaten berechnet. Die Koordinaten werden im eigenen Objekt gespeichert.
  * Der **Destruktor** schließt den Dateizeiger
  * Die **`read`-Methode** liest mit Hilfe der `read`-Funktion die Daten des Dateizeigers in eine `struct input_event` ein und analysiert diese. Je nach Art des Input-Events wird der entsprechende Wert im eigenen Objekt gespeichert. Bei den Events zu den Achs-Werten werden diese mit den gespeicherten Mittelpunktskoordinaten verrechnet und dadurch eine vorzeichenbehaftete Auslenkung vom Mittelpunkt ermittelt. Um keine Laufzeitschwierigkeiten zu erhalten wird nur ein Lesevorgang pro Aufruf der Methode durchgeführt. 