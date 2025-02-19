#!/bin/bash

interval=1

target=$2
terminator=$3

# create terminator session
screen -S terminator -d -m $terminator 115200

index=0
while [  $index -lt 30 ]; do

    # power on target
    set -x
    screen -S terminator -X stuff i
    set +x

    # wait for target connection
    while :; do
        if [[ -r ${target} ]]; then
            break
        fi
        sleep 0.5
    done

    echo "Device connected"
    sleep 0.5
    set -x

    # create target session
    screen -S ${1}_${index} -d -m -L -Logfile ${1}_${index}.puf $target 115200
    sleep 0.5

    # ask for puf
    screen -S ${1}_${index} -X stuff p
    sleep 0.5

    # kill target session
    screen -S ${1}_${index} -X kill

    # power off target
    screen -S terminator -X stuff o
    set +x

    # increment log index
    index=$((index + 1))

    # wait for target disconnection
    # commented out with USB converter
    # while [[ -r ${target} ]]; do
    #     sleep 0.5
    # done

    echo "Device disconnected"
    sleep 0.5
done

# kill terminator session
set -x
screen -S terminator -X quit
set +x


echo "File did not arrive."
exit 1
