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

#define REGISTER_MAX 8 // 8 registers should be sufficient
#define GPIO_START 0x0209C000
#define GPIO_SPACING 0x4000

int fd;
volatile uint32_t* openRegisters[REGISTER_MAX];

void cleanup();
uint32_t toRegisterIndex(uint32_t reg);

void onCleanup(int sig) {
    (void) sig;
    cleanup();
    exit(1);
}

int main() {
	// setbuf(stdout, NULL);
	
    signal(SIGINT, onCleanup);

    for (uint32_t i = 0; i < REGISTER_MAX; i++) {
        openRegisters[i] = 0;
    }

    fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    setup();

    while (1) {
        loop();
    }

    return 0;
}

void cleanup() {
    if (fd >= 0) {
        close(fd);
    }

    for (uint32_t i = 0; i < REGISTER_MAX; i++) {
        volatile uint32_t* reg = openRegisters[i];
        if (reg != 0) {
            munmap((void*)reg, GPIO_SIZE);
        }
    }
}

uint32_t toRegisterIndex(uint32_t reg) {
    // Convert to array index
    return (reg - GPIO_START) / GPIO_SPACING;
}

volatile uint32_t* getGPIO(uint32_t reg) {
    return openRegisters[toRegisterIndex(reg)];
}

void pinMode(uint32_t reg, uint32_t pin, uint8_t direction) {
    uint32_t index = toRegisterIndex(reg);
    
    volatile uint32_t* gpio = (volatile uint32_t *) mmap(NULL, GPIO_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, reg);
    if (gpio == MAP_FAILED) {
        perror("mmap");
        close(fd);
    }

    openRegisters[index] = gpio;

    if (direction == INPUT) {
        gpio[GPIO_DIR / 4] &= ~pin;
    } else {
        gpio[GPIO_DIR / 4] |= pin;
    }
}

void digitalWrite(uint32_t reg, uint32_t pin, uint8_t state) {
    volatile uint32_t* gpio = getGPIO(reg);
    if (state == LOW) {
        gpio[GPIO_DR / 4] |= pin;
    } else {
        gpio[GPIO_DR / 4] &= ~pin;
    }
}
