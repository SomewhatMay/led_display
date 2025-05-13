#pragma once

#include <stdint.h>

#define ROW_COUNT 32
#define COL_COUNT 64

#define R_ROW_COUNT 64 // Real row count - exists due to dual scan lines
#define R_COL_COUNT COL_COUNT // Real col count

/**
 * Defined as
 * 
uint8_t COLORS[8][3] = {
  {0, 0, 0},    BLACK
  {1, 1, 1},    WHITE
  {1, 0, 0},    RED
  {0, 1, 0},    GREEN
  {0, 0, 1},    BLUE
  {1, 1, 0},    YELLOW
  {0, 1, 1},    CYAN
  {1, 0, 1}     MAGENTA
};

 *
 */
extern uint8_t COLORS[8][3];

uint8_t displayState[R_ROW_COUNT][R_COL_COUNT];


void onLoad();

void onUpdate();

void paintSolid(uint8_t color);

void paintRandom();
