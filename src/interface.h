#include <stdint.h>

/* Interfacing Enums */
#define INPUT 0
#define OUTPUT 1
#define LOW 0
#define HIGH 1

#define GPIO4 0x020A8000
#define GPIO5 0x020AC000

typedef struct {
	uint32_t reg; // Register Index
	uint32_t pin; // Pin Mask
	
	volatile uint32_t* gpio; // GPIO relating to reg
} pinInfo;

// #define CLK 0x020A0000, (1 << 25) // <register>, <pin mask>
/* Pins - Indices in an array - DO NOT CHANGE */
// Even: Cable A, Odd: Cable B
#define R1 0
#define G1 1
#define B1 2
#define GND1 3 // NC
#define R2 4
#define G2 5
#define B2 6
#define E 7
#define A 8
#define B 9
#define C 10
#define D 11
#define CLK 12
#define LAT 13
#define OE 14
#define GND2 15 // NC

/* User Defined Functions*/
void setup();
void loop();

/* Interfacing Functions */
pinInfo* getPinInfo(uint32_t pin);
volatile uint32_t* getGPIO(uint32_t pin);
void pinMode(uint32_t pin, uint8_t direction);
void digitalWrite(uint32_t pin, uint8_t state);
