/** @file */

#include <errno.h>
#include <time.h>

#include "bsargame.h"

/** Hold the last error thrown. */
lbg_errno_t lbg_errno = LBG_ERRNO_SUCCESS;
/** Optional details about the last error. Can be an int i or a char array s.*/
union lbg_errno_details_u lbg_errno_details = {0};

/** Error strings */
const char *LBG_ERR_STRS[] = {
    "Failed to initialize GPIO pins\n",
    "Failed to set INPUT mode on pin %d\n",
    "Failed to initialize LEDs: %s\n",
    "Failed to render LEDs: %s\n",
    "Success\n",
};

/** @cond NO_DOC */
lbg_gpio_pin_t ALL_PINS[] = {
    LBG_GPIO_P1_A,
    LBG_GPIO_P1_B,
    LBG_GPIO_P1_UP,
    LBG_GPIO_P1_RIGHT,
    LBG_GPIO_P1_DOWN,
    LBG_GPIO_P1_LEFT,
    LBG_GPIO_P2_A,
    LBG_GPIO_P2_B,
    LBG_GPIO_P2_UP,
    LBG_GPIO_P2_RIGHT,
    LBG_GPIO_P2_DOWN,
    LBG_GPIO_P2_LEFT,
    -1,
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
                    .strip_type = WS2811_STRIP_GRB,
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

uint32_t LED_MAP[LBG_SIZE] = {0};
int LBG_PIGPIOD_IF = -1;
/** @endcond */

static bool init_gpios(void) {
    if ((LBG_PIGPIOD_IF = pigpio_start(NULL, NULL)) != 0) {
        lbg_errno = LBG_ERRNO_GPIO_INIT;
        return false;
    }

    for (lbg_gpio_pin_t i = 0; ALL_PINS[i] != (lbg_gpio_pin_t)-1; i++) {
        if (set_mode(LBG_PIGPIOD_IF, ALL_PINS[i], PI_INPUT) != 0 ||
            set_pull_up_down(LBG_PIGPIOD_IF, ALL_PINS[i], PI_PUD_DOWN) != 0 ||
            set_glitch_filter(LBG_PIGPIOD_IF, ALL_PINS[i], 200) != 0) {
            lbg_errno = LBG_ERRNO_GPIO_SETMODE;
            lbg_errno_details.i = ALL_PINS[i];
            pigpio_stop(LBG_PIGPIOD_IF);
            return false;
        }
    }
    return true;
}

static bool init_leds(void) {
    ws2811_return_t ret;

    if ((ret = ws2811_init(&LED_STRIP)) != WS2811_SUCCESS) {
        lbg_errno = LBG_ERRNO_LED_INIT;
        lbg_errno_details.s = (char *)ws2811_get_return_t_str(ret);
        pigpio_stop(LBG_PIGPIOD_IF);
        return false;
    }

    for (int i = 0; i < LBG_SIZE; i++) {
        LED_STRIP.channel[0].leds[i] = 0;
    }

    int i = 0;
    for (int y = 0; y < LBG_HEIGHT; y++) {
        for (int x = 0; x < LBG_WIDTH; x++) {
            if (!(y % 2)) {
                LED_MAP[i] = y * LBG_WIDTH + x;
            } else {
                LED_MAP[i] = y * LBG_WIDTH + (LBG_WIDTH - x - 1);
            }
            i++;
        }
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
    pigpio_stop(LBG_PIGPIOD_IF);
    ws2811_fini(&LED_STRIP);
}
