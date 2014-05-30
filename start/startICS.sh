#!/bin/bash

# ---------------------------
# Projekt: ICS - Kran Neubau
# Dateiname: startICS.sh
# Funktion: Linux-Shell-Skript zum Starten der ICS-Crane-Control-Software
# Kommentar: Funktionsfaehiges Startscript
# Parameter:
# Rueckgabe:
# Name: Andreas Dolp
# Datum: 30.05.2014
# Version: 1.0
# ---------------------------

strFilePathICSCC=/home/pi/ics-crane-control
strFilePathPID=/var/run/ics_crane_control.pid



echo "ICS - Crane-Control Startscript"
echo "Version 1.0"
echo ""
echo "PLEASE RUN SCRIPT AS ROOT!!"
echo ""

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
