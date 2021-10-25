#!/bin/sh

ARG="$0"
PROGRAM="$(dirname $ARG)/ttsnake.bin"



if [ -f "$PROGRAM" ]; then
    if [ -z "$1" ] ; then
	./$PROGRAM -d scenes
    else
	./$PROGRAM "$1"
    fi
else
    echo "Program not found."
    exit 1
fi
       
