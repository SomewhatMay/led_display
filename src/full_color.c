#include "interface.h"

#include <stdio.h>
#include <unistd.h>

#define ROW_COUNT 32
#define COL_COUNT 64

void setup() {
	setbuf(stdout, NULL);
	
	pinMode(R1, OUTPUT);
	pinMode(G1, OUTPUT);
	pinMode(B1, OUTPUT);
	
	pinMode(R2, OUTPUT);
	pinMode(G2, OUTPUT);
	pinMode(B2, OUTPUT);
	
	pinMode(A, OUTPUT);
	pinMode(B, OUTPUT);
	pinMode(C, OUTPUT);
	pinMode(D, OUTPUT);
	pinMode(E, OUTPUT);
	
	pinMode(CLK, OUTPUT);
	pinMode(LAT, OUTPUT);
	pinMode(OE, OUTPUT);
}

void selectRow(int);

int targetIndex = 0;
int direction = 1;
void loop() {
	for (int row = 0; row < ROW_COUNT; row++) {
		for (int col = 0; col < COL_COUNT; col++) {
			digitalWrite(R1, HIGH);
			digitalWrite(G1, HIGH);
			digitalWrite(B1, HIGH);
			
			digitalWrite(R2, HIGH);
			digitalWrite(G2, HIGH);
			digitalWrite(B2, HIGH); 
			
			digitalWrite(CLK, HIGH);
			digitalWrite(CLK, LOW);
		}
		
		digitalWrite(OE, HIGH);
				
		digitalWrite(LAT, HIGH);
		digitalWrite(LAT, LOW);
		
		selectRow(row);
		
		digitalWrite(OE, LOW);
	}
}

void selectRow(int row) {
	digitalWrite(A, row & 0x01);
	digitalWrite(B, row & 0x02);
	digitalWrite(C, row & 0x04);
	digitalWrite(D, row & 0x08);
	digitalWrite(E, row & 0x10);
}
