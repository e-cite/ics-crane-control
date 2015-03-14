# **Dokumentation und Erläuterung der output-Klassen** #

# **outputGPIO-Klasse** #
# Dient als Vorlage und definiert die grundlegenden Eigenschaft einer allgemeinen GPIO-Ausgabe mit den für einen Kran relevanten Signalleitungen
## Dateien ##
  * output/outputGPIO.h
  * output/outputGPIO.cpp

## Daten ##
| **Datentyp** | **Variablenname** | **Beschreibung** |
|:-------------|:------------------|:-----------------|
| `bool` | `baGPIOSignals[NUM_OF_SIGNALS]` | Array der Signalzustände, Reihenfolge siehe `#define`-Sektion der outputGPIO.h |

## Methoden ##
| **return-Typ** | **Methodenname** | **Beschreibung** |
|:---------------|:-----------------|:-----------------|
| `void` | `outputGPIO()` | Konstruktor: Initialisiert den `baGPIOSignals`-Array mit NICHT-AKTIV-Werten |
| `void` | `~outputGPIO()` | Virtueller Destruktor: Dieser wird leer definiert und dient nur zur Unterdrückung einer Compiler-Warnung |
| `bool` | `init()` | Virtuelle, abstrakte Methode zum Initialisieren der GPIO-Ausgänge, wird je nach Ableitung definiert |
| `bool` | `write()` | Virtuelle, abstrakte Methode zum Schreiben der GPIO-Ausgänge, wird je nach Ableitung definiert |
| `bool` | `setSignal(const bool *)` | Setter-Methode zum Setzen der zu schreibenden Signale, prüft diese Signale auf Konsistenz |
| `void` | `setUSBErrActive()` | Setter-Methode zum AKTIV-Setzen des USB-Fehlers und zum NICHT-AKTIV-Setzen aller übrigen Signale |
| `bool` | `getSignal(const unsigned int)` | Getter-Methode liefert Zustand des im Parameter angefragten Signals zurück |

## Beschreibung ##
  * Die abstrakte **outputGPIO-Klasse** dient nur als Vorlage für die Ableitung weiterer Klassen. Sie definiert die grundlegenden Eigenschaften einer allgemeinen GPIO-Ausgabe. Dazu zählen die digitalen Zustände der Signale sowie die zum Initialisieren, Setzen und Schreiben benötigten Methoden.
  * Die virtuelle, abstrakte **`init`-Methode** dient zum Initialisieren der GPIO-Ausgänge. Sie wird in den Ableitungen weiter definiert und unterscheidet sich je nach Device.
  * Die virtuelle, abstrakte **`write`-Methode** dient zum eigentlichen Schreiben der im Objekt gesetzten Zustände an die GPIO-Ausgänge. Sie wird in den Ableitungen weiter definiert und unterscheidet sich je nach Device.
  * Die **`setSignals(const bool *)`-Methode** dient zum Setzen der Signalzustände. **Diese werden dabei auf Konsistenz überprüft**. Im Fall des Krans dürfen nicht beide Signale einer Achse gleichzeitig AKTIV sein. Beide Signale NICHT-AKTIV hingegen ist erlaubt. Dadurch kann sichergestellt werden, dass nur gültige und nicht gegenläufige Signale an die GPIO-Ausgänge geschrieben werden. Es kann also niemals vorkommen, dass der Kran gleichzeitig nach rechts UND links schwenken soll. Waren die übergebenen Signale konsistent, so wird `TRUE` zurückgegeben.
  * Die **`setUSBErrActive()`-Methode** setzt bei Aufruf alle Signalzustände auf NICHT-AKTIV und aktiviert den USB-Fehler.
  * Mit der **`getSignals(const unsigned int)`-Methode** können die aktuellen Signalzustände abgefragt werden. Die Getter-Methode liefert den Zustand des im Parameter angefragten Signals zurück.


  * Die `#define`-Zeilen in der outputGPIO.h-Datei definieren die Position der einzelnen Signale sowie ob diese HIGH- oder LOW-aktiv sind.



# **outputGPIOsysfs-Klasse** #
# Implementiert die Methoden zum Schreiben von GPIO-Ausgängen mit Hilfe des Linux-SYSFS
## Dateien ##
  * output/outputGPIOsysfs.h
  * output/outputGPIOsysfs.cpp

## Daten ##
| **Datentyp** | **Variablenname** | **Beschreibung** |
|:-------------|:------------------|:-----------------|
| `unsigned int` | `iaGPIOPinsAddress[NUM_OF_SIGNALS]` | Array der Adressen der GPIO-Pins bzgl. der Hardware-Pin-Map (GPIO-Name, nicht Pin-Nummer auf Platine) |

## Methoden ##
| **return-Typ** | **Methodenname** | **Beschreibung** |
|:---------------|:-----------------|:-----------------|
| `void` | `outputGPIOsysfs(const unsigned int*)` | Konstruktor: Schreibt die im Parameter übergebenen Adressen der GPIO-Pins in das eigene Objekt |
| `void` | `~outputGPIOsysfs()` | Destruktor: Setzt alle Signale auf NICHT-AKTIV und aktiviert den USB-Fehler |
| `bool` | `init()` | Initialisiert die GPIO-Ausgänge durch das SYSFS: Erstellt die GPIOs und setzt deren Richtung |
| `bool` | `write()` | Schreibt die Daten an die durch `init()` initialisierten Ausgänge |

## Beschreibung ##
  * Die **outputGPIOsysfs-Klasse** ist eine public-Ableitung der outputGPIO-Klasse.
  * Der **Konstruktor** schreibt die im Parameter übergebenen Adressen der GPIO-Pins in das eigene Objekt. Diese werden zum Adressieren der Pins benötigt. **Wichtig ist dabei, dass es sich nicht um die Pin-Nummer des Pins auf der Platine handelt, sondern um die Nummer in der Bezeichnung des GPIO-Pins.**
  * Der **Destruktor** setzt alle Signale auf NICHT-AKTIV und aktiviert den USB-Fehler.
  * Die **`init()`-Methode** initialisert die GPIO-Ausgänge. Dafür wird erst jedes Device durch Schreiben der Pin-Nummer in die EXPORT-Datei erstellt. Anschließend werden aus den (_noch_) im `#define`-Bereich definierten Pfadangaben und den `iaGPIOPinsAddress[]`-Adressen die Pfade zu den GPIO-Ausgängen zusammengebaut und dort die Richtung `OUT` gesetzt. Wurde erfolgreich Initialisiert, so wird `TRUE` zurückgegeben.
  * Die **`write()`-Methode** baut sich ähnlich der `init()`-Methode die Pfade zu den GPIO-Ausgängen zusammen und schreibt den zugehörigen Wert des eigenen Objekts in die VALUE-Datei des GPIO-Ausgangs. Wurden alle Werte erfolgreich geschrieben, so wird `TRUE` zurückgegeben.