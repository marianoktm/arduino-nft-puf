#! /bin/bash

HEXFILE=$1
# HEXFILE=../optiboot_atmega328.hex
# HEXFILE=./originale_windows.hex
# HEXFILE=/usr/share/arduino/hardware/arduino/bootloaders/optiboot/optiboot_atmega328.hex

AVRDUDE=$(which avrdude)
CONF=/etc/avrdude.conf 
TTY=/dev/ttyACM0
BAUD=19200

UNLOCK=0xFF
# LOCK=0xCF
LOCK=0xFF	# nessuna restrizione
EFUSE=0xFD
# HFUSE=0xDE	# BOOTSZ[1:0]='11' BOOTRST='0'
HFUSE=0xD9	# BOOTSZ[1:0]='00' BOOTRST='1'
LFUSE=0x0xFF

$AVRDUDE -C$CONF -v -v -v -v -patmega328p -cstk500v1 -P$TTY -b$BAUD -e -Ulock:w:$UNLOCK:m -Uefuse:w:$EFUSE:m -Uhfuse:w:$HFUSE:m -Ulfuse:w:$LFUSE:m

sleep 1s

$AVRDUDE -C$CONF -v -v -v -v -patmega328p -cstk500v1 -P$TTY -b$BAUD -Uflash:w:$HEXFILE:i -Ulock:w:$LOCK:m
