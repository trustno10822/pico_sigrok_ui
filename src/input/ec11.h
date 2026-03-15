// EC11 Rotary Encoder Driver Header

#ifndef EC11_H
#define EC11_H

#include <stdint.h>
#include <stdbool.h>

// Default GPIO pin assignments for EC11 encoder
#define EC11_PIN_CLK  2   // CLK (A) signal pin
#define EC11_PIN_DT   3   // DT  (B) signal pin
#define EC11_PIN_SW   4   // Push-button switch pin

// Rotation direction values returned by ec11_get_direction()
typedef enum {
    EC11_DIR_NONE  =  0,
    EC11_DIR_CW    =  1,   // Clockwise
    EC11_DIR_CCW   = -1    // Counter-clockwise
} ec11_direction_t;

// Internal encoder state (treat as opaque outside this module)
typedef struct {
    int32_t          position;      // Accumulated step count
    ec11_direction_t last_dir;      // Most recent direction
    bool             button_pressed; // True while SW is held
    uint8_t          clk_pin;
    uint8_t          dt_pin;
    uint8_t          sw_pin;
    uint8_t          prev_ab;       // Previous CLK/DT sample for quadrature
} ec11_t;

// Initialise the encoder on the given GPIO pins.
// Call once before using any other ec11_* function.
void ec11_init(ec11_t *enc, uint8_t clk_pin, uint8_t dt_pin, uint8_t sw_pin);

// Return the accumulated step position (positive = CW, negative = CCW).
int32_t ec11_get_position(const ec11_t *enc);

// Return the direction of the most recent step.
ec11_direction_t ec11_get_direction(const ec11_t *enc);

// Return true if the push-button switch is currently pressed.
bool ec11_get_button(const ec11_t *enc);

// Reset the accumulated position counter to zero.
void ec11_reset_position(ec11_t *enc);

// Poll the encoder pins manually (call from main loop when not using interrupts).
void ec11_poll(ec11_t *enc);

#endif // EC11_H
