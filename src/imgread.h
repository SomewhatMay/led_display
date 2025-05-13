#pragma once

#include <stdint.h>

#define DATA_ROWS 64
#define DATA_COLS 8
#define DATA_PIXELS 3

/**
 * @param out of size uint8_t[DATA_ROWS][DATA_COL][DATA_PIXELS]
 */
void interpretData(const char* filename, uint8_t out[][DATA_COLS][DATA_PIXELS]);
