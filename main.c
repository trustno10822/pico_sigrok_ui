#include <stdio.h>
#include "pico/stdlib.h"
#include "input/ec11.h"

int main() {
    stdio_init_all();
    printf("Pico Sigrok UI — EC11 rotary encoder demo\n");

    ec11_t encoder;
    ec11_init(&encoder, EC11_PIN_CLK, EC11_PIN_DT, EC11_PIN_SW);

    int32_t last_position = 0;
    bool    last_button   = false;

    while (1) {
        ec11_poll(&encoder);

        int32_t pos    = ec11_get_position(&encoder);
        bool    button = ec11_get_button(&encoder);

        if (pos != last_position) {
            ec11_direction_t dir = ec11_get_direction(&encoder);
            const char *dir_str = (dir == EC11_DIR_CW)  ? "CW"  :
                                  (dir == EC11_DIR_CCW) ? "CCW" : "none";
            printf("Encoder position: %d  direction: %s\n", pos, dir_str);
            last_position = pos;
        }

        if (button != last_button) {
            printf("Button: %s\n", button ? "pressed" : "released");
            last_button = button;
        }

        sleep_ms(5);
    }

    return 0;
}
