#include "driver.h"
#include "gpio.h"

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

uint32_t currentFrameCount = 0;
clock_t start;

void selectRow(uint32_t row);
void refreshDisplay();
void incrementFramerate();

void setup() {
  setbuf(stdout, NULL);
  srand(time(NULL));
	
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
  
  paintSolid(0);
  
  start = clock();
}

void loop() {
  refreshDisplay();
  paintRandom();
  incrementFramerate();
}

void paintSolid(uint8_t color) {
  for (uint32_t row = 0; row < R_ROW_COUNT; row++) {
    for (uint32_t col = 0; col < R_COL_COUNT; col++) {
      displayState[row][col] = color;
    }
  }
}

void paintRandom() {
  for (uint32_t row = 0; row < R_ROW_COUNT; row++) {
    for (uint32_t col = 0; col < R_COL_COUNT; col++) {
      displayState[row][col] = rand() % 8;
    }
  }
}

void refreshDisplay() {
  for (uint32_t row = 0; row < ROW_COUNT; row++) {
    for (uint32_t col = 0; col < COL_COUNT; col++) {
      uint8_t *topColor = COLORS[displayState[row][col]];
      uint8_t *bottomColor = COLORS[displayState[row + 32][col]];
      
      digitalWrite(R1, topColor[0]);
      digitalWrite(G1, topColor[1]);
      digitalWrite(B1, topColor[2]);

      digitalWrite(R2, bottomColor[0]);
      digitalWrite(G2, bottomColor[1]);
      digitalWrite(B2, bottomColor[2]);

      digitalWrite(CLK, HIGH);
      digitalWrite(CLK, LOW);
    }

    digitalWrite(OE, HIGH);

    digitalWrite(LAT, HIGH);
    digitalWrite(LAT, LOW);

    selectRow(row);

    digitalWrite(OE, LOW);
  }
  
  usleep(300);
  digitalWrite(OE, HIGH);
}

void selectRow(uint32_t row) {
  digitalWrite(A, row & 0x01);
  digitalWrite(B, row & 0x02);
  digitalWrite(C, row & 0x04);
  digitalWrite(D, row & 0x08);
  digitalWrite(E, row & 0x10);
}

void incrementFramerate() {
  currentFrameCount++;
  
  if ((double) (clock() - start) / CLOCKS_PER_SEC >= 1.0) {
	  printf("FPS: %d\n", currentFrameCount);
	  
	  currentFrameCount = 0;
	  start = clock();
  } 
}
