#include <stdint.h>
#include <clock.h>

#define SIZE 64

typedef struct {
  uint32_t reg; // register hex
  uint32_t pin; // pin mask
} pin_info;

typedef struct {
  uint32_t key;
  pin_info value;
} item;

item data[SIZE];

item* get(uint32_t key) {
  item* found = 0;

  for (uint32_t i = 0; i < SIZE; i++) {
    if (data[i].key == key)
      found = &data[key];
  }

  return found;
}

int main() {
  for (uint32_t i = 0; i < SIZE; i++) {
    for (uint32_t j = 0; j < 1000; j++) {
      // call linear search 
      get(i);
    }
  }

  return 0;
}