#include "interface.h"

#include <unistd.h>
#include <stdio.h>

#define CLK_REG 0x93849A
#define CLK (1 << 25)

#define OE_REG 0x93849A
#define OE (1 << 35)

void setup() {
	pinMode(CLK, OUTPUT);
}

void loop() {
    digitalWrite(CLK, HIGH);
    digitalWrite(OE, HIGH);
    digitalWrite(LAT, HIGH);
    digitalWrite(LAT, LOW);
    
    digitalWrite(CLK_REG, CLK, HIGH);
    digitalWrite(OE_REG, CLK, HIGH);
    
    digitalWrite(0x93849A, (1 << 35), HIGH);
    digitalWrite(0x93849A, (1 << 34), HIGH);
    digitalWrite(0x93849A, (1 << 34), HIGH);
    
    usleep(500000);
    digitalWrite(CLK, LOW);
    usleep(500000);
}
