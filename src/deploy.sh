#!/bin/bash

if [ -z "$1" ]; then
	echo "Usage: $0 <filename>"
	exit 1
fi

gcc -std=gnu99 ./ledlib/gpio.c "./src/$1.c" -o app
sudo ./app
