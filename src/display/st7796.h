// ST7796 display driver header

#ifndef ST7796_H
#define ST7796_H

#include <stdint.h>

// Function prototypes for ST7796 display driver
void st7796_init();
void st7796_set_cursor(uint16_t x, uint16_t y);
void st7796_write_pixel(uint16_t color);
void st7796_fill_screen(uint16_t color);

#endif // ST7796_H
