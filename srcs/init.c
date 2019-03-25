/** @file */

#include <errno.h>

#include "bsargame.h"

/** Hold the last error thrown. */
lbg_errno_t lbg_errno = LBG_ERRNO_SUCCESS;
/** Optional details about the last error. Can be an int i or a char array s.*/
union lbg_errno_details_u lbg_errno_details = {0};

/**
 * Error strings
 */
const char *LBG_ERR_STRS[] = {
    "Failed to initialize GPIO pins\n",
    "Failed to set INPUT mode on pin %d\n",
    "Failed to initialize LEDs\n",
    "Success",
};

/** @cond NO_DOC */
// TODO: add second player
lbg_gpio_pin_t ALL_PINS[] = {
    LBG_GPIO_P1_A,
    LBG_GPIO_P1_B,
    LBG_GPIO_P1_UP,
    LBG_GPIO_P1_RIGHT,
    LBG_GPIO_P1_DOWN,
    LBG_GPIO_P1_LEFT,
    -1,
};
lbg_event_t ALL_EVENTS[] = {
    LBG_P1_A, LBG_P1_B, LBG_P1_UP, LBG_P1_RIGHT, LBG_P1_DOWN, LBG_P1_LEFT, -1,
};

ws2811_t LED_STRIP = {
    .freq = WS2811_TARGET_FREQ,
    .dmanum = LBG_LED_DMA,
    .channel =
        {
            [0] =
                {
                    .gpionum = LBG_LED_GPIO_PIN,
                    .count = LBG_SIZE,
                    .invert = 0,
                    .brightness = 255,
                    .strip_type = WS2811_STRIP_GBR,
                },
            [1] =
                {
                    .gpionum = 0,
                    .count = 0,
                    .invert = 0,
                    .brightness = 0,
                },
        },
};
/** @endcond */

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
