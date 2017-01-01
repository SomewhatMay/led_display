#include "interface.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#define GPIO_SIZE 0x1000 // Size of each register
#define GPIO_DR 0x00 // Data register
#define GPIO_DIR 0x04 // Direction register

#define PIN_MAP_SIZE 16
#define GPIO_START 0x0209C000
#define GPIO_SPACING 0x4000

// Contains a one-to-one mapping of each pin's index (see interface.h
// and an array that contains the pin's GPIO register index and pin mask
// Array size is 16 by 2 by convention
// Note: Update PIN_MAP_SIZE when updating this
pinInfo pinMap[] = {
	{GPIO5, 1 << 15, 0},
	{GPIO5, 1 << 14, 0},
	{GPIO5, 1 << 13, 0},
	{GPIO5, 1 << 12, 0}, // NC
	{GPIO5, 1 << 11, 0},
	{GPIO5, 1 << 10, 0},
	{GPIO5, 1 << 9, 0},
	{GPIO5, 1 << 8, 0},
	{GPIO5, 1 << 7, 0},
	{GPIO5, 1 << 6, 0},
	{GPIO5, 1 << 5, 0},
	{GPIO4, 1 << 31, 0},
	{GPIO4, 1 << 30, 0},
	{GPIO4, 1 << 29, 0},
	{GPIO4, 1 << 28, 0},
	{GPIO4, 1 << 27, 0} // NC
};

int fd;

void cleanup(int sig) {
    (void) sig;
    
    // Close open file
    if (fd >= 0) {
        close(fd);
    }
    
    // Remove each register's virtual memory allocation
    for (uint32_t i = 0; i < PIN_MAP_SIZE; i++) {
        pinInfo* current = &pinMap[i];
        
        int closed = 0;
        for (uint32_t j = 0; j < i && !closed; j++) {
			if (pinMap[j].reg == current->reg) {
				closed = 1;
			}
		}
		
		if (!closed) {
			munmap((void*) current->reg, GPIO_SIZE);
		}
    }
    
    exit(1);
}

int main() {
	// setbuf(stdout, NULL);
	
    signal(SIGINT, cleanup);

    fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd < 0) {
        perror("open");
        return 1;
    }
    
    // Open all registers
    for (uint32_t i = 0; i < PIN_MAP_SIZE; i++) {
        pinInfo* info = &(pinMap[i]);
        volatile uint32_t* gpio = 0;
        
        // Verify that current register is not already open
        int open = 0;
        for (int j = 0; j < i && !open; j++) {
			if (pinMap[j].reg == info->reg) {
				open = 1;
				gpio = pinMap[j].gpio;
			}
		}
		
		if (!open) {
			// Open the register
			gpio = (volatile uint32_t *) mmap(NULL, GPIO_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, info->reg);
			
			if (gpio == MAP_FAILED) {
				perror("mmap");
				close(fd);
				return 1;
			}
		}
		
		// assign the register to the GPIO
		info->gpio = gpio;
    }
    
    
    printf("Loaded Pins:\n");
    for (uint32_t i = 0; i < PIN_MAP_SIZE; i++) {
		pinInfo* info = &pinMap[i];
		printf("Pin info: %d, %d, %p\n", info->reg, info->pin, (void*) info->gpio);
	}
	printf("=============\n");

    setup();
    loop();
    
    while (1) {
        loop();
    }

    return 0;
}

pinInfo* getPinInfo(uint32_t pin) {
	return &(pinMap[pin]);
}

volatile uint32_t* getGPIO(uint32_t pin) {
    return pinMap[pin].gpio;
}

void pinMode(uint32_t pin, uint8_t direction) {
	pinInfo* info = getPinInfo(pin);
	volatile uint32_t* gpio = getGPIO(pin);
	
    if (direction == INPUT) {
        gpio[GPIO_DIR / 4] &= ~(info->pin);
    } else {
        gpio[GPIO_DIR / 4] |= info->pin;
    }
}

void digitalWrite(uint32_t pin, uint8_t state) {
	pinInfo* info = getPinInfo(pin);
    volatile uint32_t* gpio = getGPIO(pin);
    
    if (state == LOW) {
		gpio[GPIO_DR / 4] &= ~(info->pin);
    } else {
        gpio[GPIO_DR / 4] |= info->pin;
    }
}
