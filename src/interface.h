#include <stdint.h>

/* Interfacing Enums */
#define INPUT 0
#define OUTPUT 1
#define LOW 0
#define HIGH 1

typedef struct {
	uint32_t reg; // Register Index
	uint32_t pin; // Pin Mask
	
	volatile uint32_t* gpio; // GPIO relating to reg
} pinInfo;

// #define CLK 0x020A0000, (1 << 25) // <register>, <pin mask>
/* Pins - Indices in an array - DO NOT CHANGE */
#define R1 0

/* User Defined Functions*/
void setup();
void loop();

/* Interfacing Functions */
pinInfo* getPinInfo(uint32_t pin);
volatile uint32_t* getGPIO(uint32_t pin);
void pinMode(uint32_t pin, uint8_t direction);
void digitalWrite(uint32_t pin, uint8_t state);
