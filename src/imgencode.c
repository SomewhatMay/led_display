#include "imgread.h"

#include <stdint.h>
#include <stdio.h>

#define OUT_SIZE DATA_ROWS * DATA_COLS * DATA_PIXELS

int main() {
  FILE *file = fopen("alternating.dat", "wb");
  
  if (file == NULL) {
    perror("Error opening file");
    return 1;
  }
  
  uint8_t data[OUT_SIZE];
  
  for (uint32_t i = 0; i < OUT_SIZE; i++) {
    data[i] = i % 2 == 0 ? 1 : 0;
  }
  
  printf("Data to be written:\n");  
  printf("   ");
  for (uint32_t col = 0; col < DATA_COLS; col++) {
    printf("%d: ", col);
  }
  printf("\n");
  
  for (uint32_t row = 0; row < DATA_ROWS; row++) {
    printf("%d: ", row);
    for (uint32_t col = 0; col < DATA_COLS; col++) {
      for (uint32_t pix = 0; pix < DATA_PIXELS; pix++) {
        printf("%d ", data[row * (DATA_COLS * DATA_PIXELS) + col * DATA_PIXELS + pix]);
      }
      
      printf("   ");
    }
    
    printf("\n");
  }
  
  fwrite(data, 1, sizeof(data), file);
  
  fclose(file);
  return 0;
}
