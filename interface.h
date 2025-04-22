#include <stdint.h>

/* Interfacing Enums */
#define INPUT 0
#define OUTPUT 1
#define LOW 0
#define HIGH 1

/* Pins */
#define CLK 0x020A0000, (1 << 25) // <register>, <pin mask>

/* User Defined Functions*/
void setup();
void loop();

/* Interfacing Functions */
volatile uint32_t* getGPIO(uint32_t reg);
void pinMode(uint32_t reg, uint32_t pin, uint8_t direction);
void digitalWrite(uint32_t reg, uint32_t pin, uint8_t state);
