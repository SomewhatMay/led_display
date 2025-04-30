#!/bin/bash

if [ "$EUID" -ne 0 ]; then
  echo "Please run as root"
  exit 1
fi

gcc -std=gnu99 interface.c $1.c -o app
sudo ./app