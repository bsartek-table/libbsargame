#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <ws2811.h>

#if defined(DEBUG) || defined(RELEASE)
#else
#define DEBUG
#endif

/* Public */
#define LBG_WIDTH 13
#define LBG_HEIGHT 21
#define LBG_FRAMERATE 20

#define lbg_debug(fmt, ...)                                                    \
    do {                                                                       \
        if (DEBUG)                                                             \
            fprintf(stderr, "%s:%d: %s", __FILE__, __LINE__, fmt,              \
                    ##__VA_ARGS__);                                            \
    } while (0)

typedef enum lbg_color_e {
    LBG_BLACK = 0x0,
    LBG_WHITE = 0x00202020,
    LBG_RED = 0x00200000,
    LBG_YELLOW = 0x00202000,
    LBG_GREEN = 0x00002000,
    LBG_LIGHTBLUE = 0x00002020,
    LBG_BLUE = 0x00000020,
    LBG_PURPLE = 0x00100010,
    LBG_ORANGE = 0x00201000,
    LBG_PINK = 0x00200010,
} lbg_color_t;

typedef enum lbg_event_e {
    LBG_P1_UP,
    LBG_P1_RIGHT,
    LBG_P1_DOWN,
    LBG_P1_LEFT,
    LBG_P2_UP,
    LBG_P2_RIGHT,
    LBG_P2_DOWN,
    LBG_P2_LEFT,
    LBG_P1_A,
    LBG_P1_B,
    LBG_P2_A,
    LBG_P2_B,
} lbg_event_t;

typedef enum lbg_errno_e {
    LBG_ERRNO_GPIO_INIT,
    LBG_ERRNO_GPIO_SETMODE,
    LBG_ERRNO_LED_INIT,
    LBG_ERRNO_SIGACTION,
    LBG_ERRNO_SUCCESS
} lbg_errno_t;

const char *LBG_ERR_STRS[] = {
    "Failed to initialize GPIO pins\n",
    "Failed to set INPUT mode on pin %d\n",
    "Failed to initialize LEDs\n",
    "Failed to setup signal handlers: %s\n",
    "Success",
};

bool lbg_init(void);
void lbg_exit(void);
bool lbg_get_screen(int32_t *screen);
bool lbg_update_screen(int32_t *screen);
bool lbg_poll_event(lbg_event_t *event);
void lbg_perror(void);

lbg_errno_t lbg_errno = LBG_ERRNO_SUCCESS;
union {
    int i;
    char *s;
} lbg_errno_details = {0};

/* Private */

#define LBG_LED_GPIO_PIN 18
#define LBG_LED_DMA 10

// TODO: check pins
typedef enum lbg_gpio_pin_e {
    LBG_GPIO_P1_A = 17,
    LBG_GPIO_P1_B = 27,
    LBG_GPIO_P1_UP = 10,
    LBG_GPIO_P1_RIGHT = 9,
    LBG_GPIO_P1_DOWN = 5,
    LBG_GPIO_P1_LEFT = 11,
} lbg_gpio_pin_t;

lbg_gpio_pin_t ALL_PINS[] = {
    LBG_GPIO_P1_A,
    LBG_GPIO_P1_B,
    LBG_GPIO_P1_UP,
    LBG_GPIO_P1_RIGHT,
    LBG_GPIO_P1_DOWN,
    LBG_GPIO_P1_LEFT,
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
                    .count = LBG_WIDTH * LBG_HEIGHT,
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
