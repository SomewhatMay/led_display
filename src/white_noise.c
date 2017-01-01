#include "driver.h"
#include "imgread.h"

#include <stdlib.h>

void onLoad() {
  
}

void onUpdate() {
  for (uint32_t row = 0; row < R_ROW_COUNT; row++) {
    for (uint32_t col = 0; col < R_COL_COUNT; col++) {
      displayState[row][col] = rand() % 2;
    }
  }
}
