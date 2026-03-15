// EC11 Rotary Encoder Driver Implementation
//
// The EC11 produces two quadrature signals (CLK/A and DT/B) whose phase
// relationship indicates both the step count and the rotation direction.
// A full Gray-code transition table is used so that the decoder handles
// both full-step and half-step detents reliably.
//
// Pin wiring (encoder side → Pico GPIO):
//   CLK  → EC11_PIN_CLK (default GPIO 2)
//   DT   → EC11_PIN_DT  (default GPIO 3)
//   SW   → EC11_PIN_SW  (default GPIO 4)
//   GND  → GND
// All three pins use the internal pull-up resistor.

#include "ec11.h"

#include "pico/stdlib.h"
#include "hardware/gpio.h"

// ---------------------------------------------------------------------------
// Gray-code quadrature transition table
// Index: (prev_AB << 2) | curr_AB  (4 bits → 16 entries)
// Value: step delta (-1, 0, or +1)
// ---------------------------------------------------------------------------
static const int8_t QEM_TABLE[16] = {
     0, -1,  1,  0,   // prev=00 → curr 00,01,10,11
     1,  0,  0, -1,   // prev=01 → curr 00,01,10,11
    -1,  0,  0,  1,   // prev=10 → curr 00,01,10,11
     0,  1, -1,  0    // prev=11 → curr 00,01,10,11
};

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

static uint8_t read_ab(const ec11_t *enc) {
    uint8_t clk = gpio_get(enc->clk_pin) ? 1u : 0u;
    uint8_t dt  = gpio_get(enc->dt_pin)  ? 1u : 0u;
    return (clk << 1) | dt;
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

void ec11_init(ec11_t *enc, uint8_t clk_pin, uint8_t dt_pin, uint8_t sw_pin) {
    enc->clk_pin       = clk_pin;
    enc->dt_pin        = dt_pin;
    enc->sw_pin        = sw_pin;
    enc->position      = 0;
    enc->last_dir      = EC11_DIR_NONE;
    enc->button_pressed = false;

    // Configure CLK and DT pins as inputs with pull-ups
    gpio_init(clk_pin);
    gpio_set_dir(clk_pin, GPIO_IN);
    gpio_pull_up(clk_pin);

    gpio_init(dt_pin);
    gpio_set_dir(dt_pin, GPIO_IN);
    gpio_pull_up(dt_pin);

    // Configure SW pin as input with pull-up
    gpio_init(sw_pin);
    gpio_set_dir(sw_pin, GPIO_IN);
    gpio_pull_up(sw_pin);

    // Capture initial state so the first poll has a valid baseline
    enc->prev_ab = read_ab(enc);
}

int32_t ec11_get_position(const ec11_t *enc) {
    return enc->position;
}

ec11_direction_t ec11_get_direction(const ec11_t *enc) {
    return enc->last_dir;
}

bool ec11_get_button(const ec11_t *enc) {
    return enc->button_pressed;
}

void ec11_reset_position(ec11_t *enc) {
    enc->position = 0;
    enc->last_dir = EC11_DIR_NONE;
}

void ec11_poll(ec11_t *enc) {
    // --- Quadrature decode ---
    uint8_t curr_ab = read_ab(enc);
    uint8_t index   = (enc->prev_ab << 2) | curr_ab;
    int8_t  delta   = QEM_TABLE[index];

    if (delta != 0) {
        enc->position += delta;
        enc->last_dir  = (delta > 0) ? EC11_DIR_CW : EC11_DIR_CCW;
    }
    enc->prev_ab = curr_ab;

    // --- Button state (active-low with pull-up) ---
    enc->button_pressed = !gpio_get(enc->sw_pin);
}
