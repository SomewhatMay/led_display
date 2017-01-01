#include "driver.h"
#include "imgread.h"

#include <stdio.h>

void onLoad() {
  
}

void onUpdate() {
  uint8_t data[DATA_ROWS][DATA_COLS][DATA_PIXELS];
  interpretData("alternating.dat", data);
  
  printf("Raw:\n");  
  printf("   ");
  for (uint32_t col = 0; col < DATA_COLS; col++) {
    printf("%d: ", col);
  }
  printf("\n");
  
  for (uint32_t row = 0; row < DATA_ROWS; row++) {
    printf("%d: ", row);
    for (uint32_t col = 0; col < DATA_COLS; col++) {
      for (uint32_t pix = 0; pix < DATA_PIXELS; pix++) {
        printf("%d ", data[row * DATA_COLS + col * DATA_PIXELS + pix]);
      }
      
      printf("   ");
    }
    
    printf("\n");
  }

  printf("Complete\n");
}
