#!/bin/sh
project_name=$(basename "$PWD")

if [ "$1" = "l" ]
then
	DSLite load -c tms570ls3137.ccxml -f ./build/$project_name.out
elif [ "$1" = "b" ]
then
	if ! [ -d "./build" ]; then mkdir build; fi 
	echo "--->run cmake"
	cmake . build
	echo "--->run make"
	make -C ./build
elif [ "$1" = "s" ]
then
	minicom -D /dev/tty.usbserial-TIMCUHDK1 -b 9600
else
	echo "command failed! l:load b:build s:serial"
fi