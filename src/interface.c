#include "interface.h"

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define GPIO_SIZE 0x1000 // Size of each register
#define GPIO_DR 0x00     // Data register
#define GPIO_DIR 0x04    // Direction register

#define PIN_MAP_SIZE 16 // Depends on the number of connections

/**
 * Contains a one-to-one mapping between each pin's hard-coded index
 * (see interface.h) and pin_info object that contains the pin's register's
 * memory address, a pin mask, and the pointer to the aforementioned
 * register's mapped memory address.
 *
 * @note Update PIN_MAP_SIZE pinMap's size changes
 */
pin_info pinMap[] = {
    {GPIO5, 1 << 15, 0}, {GPIO5, 1 << 14, 0}, {GPIO5, 1 << 13, 0},
    {GPIO5, 1 << 12, 0}, // NC
    {GPIO5, 1 << 11, 0}, {GPIO5, 1 << 10, 0}, {GPIO5, 1 << 9, 0},
    {GPIO5, 1 << 8, 0},  {GPIO5, 1 << 7, 0},  {GPIO5, 1 << 6, 0},
    {GPIO5, 1 << 5, 0},  {GPIO4, 1 << 31, 0}, {GPIO4, 1 << 30, 0},
    {GPIO4, 1 << 29, 0}, {GPIO4, 1 << 28, 0}, {GPIO4, 1 << 27, 0} // NC
};

// Handle to /dev/mem
int fd;

/**
 * Deallocates mapped virtual memory and closes all opened files.
 */
void cleanup(int sig) {
  // Unused variable warning suppression
  (void)sig;

  // Close memory directory handle
  if (fd >= 0) {
    close(fd);
  }

  // Free each register's virtual memory mapping
  for (uint32_t i = 0; i < PIN_MAP_SIZE; i++) {
    pin_info *current = &pinMap[i];

    int closed = 0;
    for (uint32_t j = 0; j < i && !closed; j++) {
      if (pinMap[j].reg == current->reg) {
        closed = 1;
      }
    }

    if (!closed) {
      munmap((void *)current->reg, GPIO_SIZE);
    }
  }

  // Close due to interrupt
  exit(1);
}

int main() {
  // setbuf(stdout, NULL);

  // Bind cleanup code in case of an unexpected crash
  signal(SIGINT, cleanup);

  // Attain a handle to the system's RAM
  fd = open("/dev/mem", O_RDWR | O_SYNC);
  if (fd < 0) {
    perror("open");
    return 1;
  }

  // Create a mapping to all registers in pinMap
  for (uint32_t i = 0; i < PIN_MAP_SIZE; i++) {
    pin_info *info = &(pinMap[i]);
    volatile uint32_t *gpio = 0;

    // Verify that the current register does not already have a mapping.
    // If it is, we can reuse the mapping
    int open = 0;
    for (int j = 0; j < i && !open; j++) {
      if (pinMap[j].reg == info->reg) {
        open = 1;
        gpio = pinMap[j].gpio;
      }
    }

    if (!open) {
      // Create a mapping to the memory register
      gpio = (volatile uint32_t *)mmap(NULL, GPIO_SIZE, PROT_READ | PROT_WRITE,
                                       MAP_SHARED, fd, info->reg);

      if (gpio == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
      }
    }

    // Store the mapped GPIO (reference to memory register)
    info->gpio = gpio;
  }

  /*
                      for (uint32_t i = 0; i < PIN_MAP_SIZE; i++) {
                      pinInfo* info = &pinMap[i];
                      printf("Pin info: %d, %d, %p\n", info->reg, info->pin,
     (void*) info->gpio);
              }
              */

  setup();

  while (1) {
    loop();
  }

  return 0;
}

pin_info *getPinInfo(uint32_t pin) { return &(pinMap[pin]); }

volatile uint32_t *getGPIO(uint32_t pin) { return pinMap[pin].gpio; }

void pinMode(uint32_t pin, uint8_t direction) {
  pin_info *info = getPinInfo(pin);
  volatile uint32_t *gpio = getGPIO(pin);

  if (direction == INPUT) {
    gpio[GPIO_DIR / 4] &= ~(info->pin);
  } else {
    gpio[GPIO_DIR / 4] |= info->pin;
  }
}

void digitalWrite(uint32_t pin, uint8_t state) {
  pin_info *info = getPinInfo(pin);
  volatile uint32_t *gpio = getGPIO(pin);

  if (state == LOW) {
    gpio[GPIO_DR / 4] &= ~(info->pin);
  } else {
    gpio[GPIO_DR / 4] |= info->pin;
  }
}
