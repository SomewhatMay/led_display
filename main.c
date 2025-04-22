#include "interface.h"

#include <unistd.h>
#include <stdio.h>

void setup() {
	pinMode(CLK, OUTPUT);
}

void loop() {
    digitalWrite(CLK, HIGH);
    usleep(500000);
    digitalWrite(CLK, LOW);
    usleep(500000);
}
