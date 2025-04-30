#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>
#include <signal.h>

#define OUTPUT 0
#define INPUT 1

#define HIGH 1
#define LOW 0

// Total size of GPIO register space to map (4kb)
#define GPIO_SIZE 0x1000

// Data Register that lets us read or write to the pin values directly.
// Each gpio uint32_t* represents an array of 32-bit integers where each 
// 4-byte section represents a different command. The first index (the 
// first four bytes) represent the data register. Whatever you write to 
// the data register gets replicated to the pin in real-time.
#define GPIO_DATAR 0x00

// Direction Register - Controls whether each pin is input (0) or output (0)
#define GPIO_DIRR 0x04

// The maximum number of GPIOs the program can open
#define MAX_GPIO_COUNT 16

/* Methods Implemented by User */
void setup();
void loop();

/* Program Variables */

// File object used to access /dev/mem/
int fd;

// Array of GPIO address arrays (2D Array)
// 16 registers should be plenty - remember, each register can
// control 31 pins
volatile uint32_t *gpios[MAX_GPIO_COUNT];
int gpios_size = 0;

// Contains information about a pin's register and mask
typedef struct {
	uint32_t reg; // GPIO base - register identifier (e.g. GPIO2)
	uint32_t pin; // pin mask (e.g. 1 << 25 accesses pin 25 in register)
} pin_info;

/* Exposed Methods */

/**
 * Returns a pin number given the pcb pin number
 */
pin_info* mapPin(uint32_t pcb_pin);

/**
 * Sets the direction of pin_mask in gpio_base register to a direction
 * (INPUT or OUTPUT)
 * 
 * @param gpio_base the memory address for this pin's register
 * @param pin_mask the bitmask that chooses a specific pin (NOT the 
 * same as pin number on PCB)
 * @param direction either OUTPUT or INPUT
 * 
 * @example choose pin 50 on PCB
 * // Lives on IO2_xIO
 * // Pin 25 as per the datasheet, bit 25 represents the pin.
 * pinMode(0x020A0000 lives on IO2, (1 << 25), OUTPUT);
 */
void pinMode(uint32_t gpio_base, uint32_t pin_mask, uint8_t direction);

/**
 * A more user-friendly version of the pinMode method. Maps known
 * pcb pin numbers to their corresponding registers and pin masks before
 * setting the pin mode
 */
void pinMode(uint32_t pcb_pin);

/* Restores OS state once program ends */
void cleanup(int sig) {
	(void)sig;
	
	close(fd);
	
	// unmap memory
	for (uint32_t i = 0; i < gpios_size; i++) {
		munmap((void*) gpios[i], GPIO_SIZE);
	}
	
	exit(1);
}

int main(void) {
	signal(SIGINT, cleanup);
	
	fd = open("/dev/mem", O_RDWR | O_SYNC);
	if (fd < 0) {
		perror("open");
		return 1;
	}
	
	
	
	return 0;
}


void pinMode(uint32_t gpio_base, uint32_t pin_mask, uint8_t direction) {
	gpio = (volatile uint32_t*) mmap(NULL, GPIO_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO_BASE);
	if (gpio == MAP_FAILED) {
		perror("mmap");
		close(fd);
		return 1;
	}
	
	gpio[GPIO_GDIR / 4] |= GPIO_PIN; 
	
	int found = 0;
	for (uint32_t i = 0; i < gpios_size; i++) {
		if (gpios[i] == gpio_base) {
			found = 1;
		}
	}
}
