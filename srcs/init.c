/** @file */

#include <errno.h>

#include "bsargame.h"

static bool init_gpios(void) {
    if (gpioInitialise() == PI_INIT_FAILED) {
        lbg_errno = LBG_ERRNO_GPIO_INIT;
        return false;
    }

    for (lbg_gpio_pin_t i = 0; ALL_PINS[i] != (lbg_gpio_pin_t)-1; i++) {
        if (gpioSetMode(ALL_PINS[i], PI_INPUT) != 0) {
            lbg_errno = LBG_ERRNO_GPIO_SETMODE;
            lbg_errno_details.i = ALL_PINS[i];
            gpioTerminate();
            return false;
        }
    }
    return true;
}

static bool init_leds(void) {
    if (ws2811_init(&LED_STRIP) != WS2811_SUCCESS) {
        lbg_errno = LBG_ERRNO_LED_INIT;
        gpioTerminate();
        return false;
    }
    return true;
}

bool lbg_init(void) {
    if (!init_gpios() || !init_leds()) {
        return false;
    }

    return true;
}

void lbg_exit(void) {
    gpioTerminate();
    ws2811_fini(&LED_STRIP);
}
