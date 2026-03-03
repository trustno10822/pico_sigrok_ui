#include <stdio.h>
#include "pico/stdlib.h"

int main() {
    stdio_init_all();
    printf("Hello from Pico Sigrok UI!\n");
    
    while (1) {
        printf("Running...\n");
        sleep_ms(1000);
    }
    
    return 0;
}
