#include "gpio.h"

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#define ROW_COUNT 32
#define COL_COUNT 64

#define R_ROW_COUNT 64 // Real row count
#define R_COL_COUNT COL_COUNT // Real col count

uint8_t COLORS[][3] = {
  {0, 0, 0},
  {1, 1, 1},
  {1, 0, 0},
  {0, 1, 0},
  {0, 0, 1},
  {1, 1, 0},
  {0, 1, 1},
  {1, 0, 1}
};

uint8_t displayState[R_ROW_COUNT][R_COL_COUNT];

uint32_t currentFrameCount = 0;
clock_t start;

void selectRow(uint32_t row);
void refreshDisplay();
void incrementFramerate();

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
  
  for (uint32_t row = 0; row < R_ROW_COUNT; row++) {
    for (uint32_t col = 0; col < R_COL_COUNT; col++) {
      displayState[row][col] = 0;
    }
  }
  
  start = clock();
  
  for (uint32_t row = 0; row < R_ROW_COUNT; row++) {
    for (uint32_t col = 0; col < R_COL_COUNT; col++) {
      
      displayState[row][col] = 1;
    }
  }
}

void loop() {
  refreshDisplay();
  //incrementFramerate();
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
