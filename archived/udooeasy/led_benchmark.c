/** 
 * Author: Umayeer Ahsan
 * Project: LED Blink
 * Date: April 2025
 * 
 * A simple blink program to drive an LED connected to 
 * Physical Pin 50 on the UDOO DUAL/QUAD.
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>

// Memory address for GPIO2 on i.MX6
// GPIO2_IO25 Lives in GPIO2, so we must use this range
#define GPIO_BASE 0x020A0000

// Total size of GPIO register space to map (4kb)
#define GPIO_SIZE 0x1000

// Data Register that lets us read or write to the pin values directly.
// Each gpio uint32_t* represents an array of 32-bit integers where each 
// 4-byte section represents a different command. The first index (the 
// first four bytes) represent the data register. Whatever you write to 
// the data register gets replicated to the pin in real-time.
#define GPIO_DR 0x00

// Direction Register - Controls whether each pin is input (0) or output (0)
#define GPIO_GDIR 0x04

// Controlling IO25 (see tinyurl.com/udooGPIO)
// Each 32-bit register can control many pins: pins IO0 through IO31
// This bitmask chooses IO25 specifically. 
#define GPIO_PIN (1 << 25)

double duration = 5.0; // in seconds

int main() {
	uint32_t i;
	clock_t start;
	int fd;
	volatile uint32_t *gpio;
	
	// Open /dev/mem with read and write access with synchronization
	fd = open("/dev/mem", O_RDWR | O_SYNC);
	if (fd < 0) {
		perror("open");
		return 1;
	}
	
	// Memory map the GPIO2 registers to our program's process space so we can safely edit it
	// gpio now points to the virtual memory that corresponds to the actual GPIO hardware registers
	// Cast to volatile so compiler knows not to optimize this, since values can change
	gpio = (volatile uint32_t*) mmap(NULL, GPIO_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO_BASE);
	if (gpio == MAP_FAILED) {
		perror("mmap");
		close(fd);
		return 1;
	}
	
	gpio[GPIO_GDIR / 4] |= GPIO_PIN; 
	
	start = clock();
	while ((double)(clock() - start) / CLOCKS_PER_SEC < duration) {
		// Bitwise OR with the pin 25 bitmask - sets pin 25 HIGH and 
		// leaves other pins as they are
		gpio[GPIO_DR] |= GPIO_PIN; 
		
		// Bitwise AND with the pin 25 bitmask - sets pin 25 LOW and
		// leaves other pins as they are
		gpio[GPIO_DR] &= ~GPIO_PIN;
		i++;
	}
	
	printf("Iterations per second: %.3lf\n", i / duration);
	
	munmap((void*) gpio, GPIO_SIZE);
	close(fd);
	

	return 0;
}
