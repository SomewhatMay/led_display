#pragma once

#include <stdint.h>

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

void paintSolid(uint8_t color);
void paintRandom();
