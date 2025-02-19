#!/bin/bash

if [ "$1" != "" ]; then
	HEXFILE=$1
else
    echo "non hai specificato lo sketch"
	exit 0
fi


AVRDUDE=/usr/share/arduino/hardware/tools/avrdude  
CONF=/usr/share/arduino/hardware/tools/avrdude.conf 
TTY=/dev/ttyACM0
BAUD=115200

$AVRDUDE -C$CONF -v -v -v -v -patmega328p -carduino -P$TTY -b$BAUD -D -Uflash:w:$HEXFILE:i
