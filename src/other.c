#include "interface.h"

#include <stdio.h>
#include <unistd.h>

void setup() {
	setbuf(stdout, NULL);
	
	pinMode(R1, OUTPUT);
}

void loop() {
	digitalWrite(R1, HIGH);
	//printf("HIGH\n");
	//usleep(500000);
	digitalWrite(R1, LOW);
	//printf("LOW\n");
	//usleep(500000);
}
