// GT911 Capacitive Touch Driver Header

#ifndef GT911_H
#define GT911_H

// Include necessary headers
#include <stdint.h>

// Define constants
#define GT911_I2C_ADDRESS 0x5D

// Function prototypes
void gt911_init(void);
uint8_t gt911_read_data(uint8_t *data, uint16_t length);

#endif // GT911_H
