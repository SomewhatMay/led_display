#include "imgread.h"

#include <stdint.h>
#include <stdio.h>

void interpretData(const char* filename, uint8_t out[][DATA_COLS][DATA_PIXELS]) {
  setbuf(stdout, NULL);
  
  FILE *file = fopen(filename, "rb");
  
  if (file == NULL) {
    perror("Unable to open file");
    return;
  }
  
  uint8_t buffer[DATA_ROWS * DATA_COLS * DATA_PIXELS];
  
  fread(buffer, 1, sizeof(buffer), file);
  
  printf("Direct Raw\n");
  for (uint32_t i = 0; i < sizeof(buffer); i++) {
    printf("%d", buffer[i]);
  }
  printf("\n");
  
  for (uint32_t row = 0; row < DATA_ROWS; row++) {
    for (uint32_t col = 0; col < DATA_COLS; col++) {
      for (uint32_t pix = 0; pix < DATA_PIXELS; pix++) {
        out[row][col][pix] = buffer[row * (DATA_COLS * DATA_PIXELS) + col * DATA_PIXELS + pix];
      }
    }
  }
  
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
        printf("%d ", buffer[row * DATA_COLS + col * DATA_PIXELS + pix]);
      }
      
      printf("   ");
    }
    
    printf("\n");
  }
  
  /*
  printf("Formatted:\n");
  for (uint32_t row = 0; row < DATA_ROWS; row++) {
    for (uint32_t col = 0; col < DATA_COLS; col++) {
      for (uint32_t pix = 0; pix < DATA_PIXELS; pix++) {
        printf("%d", out[row][col][pix]);
        printf(" ");
      }
      printf("   ");
    }
    printf("\n");
  }
  */
  printf("Complete\n");
}

int main() {
  uint8_t data[DATA_ROWS][DATA_COLS][DATA_PIXELS];
  
  for (uint32_t row = 0; row < DATA_ROWS; row++) {
    for (uint32_t col = 0; col < DATA_COLS; col++) {
      for (uint32_t pix = 0; pix < DATA_PIXELS; pix++) {
        data[row][col][pix] = 0;
      }
    }
  }
  
  interpretData("alternating.dat", data);
  
  return 0;
}
