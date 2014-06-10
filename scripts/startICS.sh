#!/bin/sh

# ---------------------------
# Projekt: ICS - Kran Neubau
# Dateiname: startICS.sh
# Funktion: Linux-Shell-Skript zum Starten der ICS-Crane-Control-Software
# Kommentar: Anpassung an aktuelle Ordnerstruktur, Verbesserungen im Ablauf
# Parameter:
# Rueckgabe:
# Name: Andreas Dolp
# Datum: 06.06.2014
# Version: 1.1
# ---------------------------

### BEGIN INIT INFO
# Provides:		ics-crane-control
# Required-Start:	
# Required-Stop:	
# Default-Start:	2
# Default-Stop:		1
# Short-Description:   ICS-crane-control
### END INIT INFO

strFilePathICSCC=/home/pi/ics-crane-control/bin
strFilePathPID=/var/run/ics_crane_control.pid

echo ""

cd $strFilePathICSCC

case "$1" in
	start)		echo "starting ics-crane-control..."
			$strFilePathICSCC/ics-crane-control_release &
			echo $! > $strFilePathPID
			;;
	debug_ncurses)	echo "starting ics-crane-control ncurses-DEBUG..."
			$strFilePathICSCC/ics-crane-control_ncurses
			;;
	debug_printf)	echo "starting ics-crane-control printf-DEBUG..."
			$strFilePathICSCC/ics-crane-control_printf
			;;
	stop)		echo "stopping ics-crane-control..."
			kill `cat $strFilePathPID`
			rm $strFilePathPID
			;;
			
	*)		echo "USAGE: start | debug_ncurses | debug_printf | stop"
			;;
esac

return 0
