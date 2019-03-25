/** @file */

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <pigpio.h>
#include <ws2811.h>

/** @cond NO_DOC */
#if defined(DEBUG) || defined(RELEASE)
#else
#define DEBUG
#endif
/** @endcond */

#define LBG_WIDTH 13                      /*!< Screen's width */
#define LBG_HEIGHT 21                     /*!< Screen's height */
#define LBG_SIZE (LBG_HEIGHT * LBG_WIDTH) /*!< Screen's size */
#define LBG_FRAMERATE 20                  /*!< Screen's framerate */
#define LBG_MAX_EVENTS 60 /*!< Maximum number of events stored in the queue */

/**
 * Debug macro
 *
 * @param fmt printf-like format string
 * @param ... variadic arguments
 */
#define lbg_debug(fmt, ...)                                                    \
    do {                                                                       \
        if (DEBUG)                                                             \
            fprintf(stderr, "%s:%d: %s", __FILE__, __LINE__, fmt,              \
                    ##__VA_ARGS__);                                            \
    } while (0)

/**
 * Most commonly used colors
 */
typedef enum lbg_color_e {
    LBG_BLACK = 0x0 /*!< Black */,
    LBG_WHITE = 0x00202020 /*!< White */,
    LBG_RED = 0x00200000 /*!< Red */,
    LBG_YELLOW = 0x00202000 /*!< Yellow */,
    LBG_GREEN = 0x00002000 /*!< Green */,
    LBG_LIGHTBLUE = 0x00002020 /*!< Lightblue */,
    LBG_BLUE = 0x00000020 /*!< Blue */,
    LBG_PURPLE = 0x00100010 /*!< Purple */,
    LBG_ORANGE = 0x00201000 /*!< Orange */,
    LBG_PINK = 0x00200010 /*!< Pink */,
} lbg_color_t;

/**
 * Available events
 */
typedef enum lbg_event_e {
    LBG_P1_A /*!< First player's A button */,
    LBG_P1_B /*!< First player's B button */,
    LBG_P1_UP /*!< First player's UP joystick */,
    LBG_P1_RIGHT /*!< First player's RIGHT joystick */,
    LBG_P1_DOWN /*!< First player's DOWN joystick */,
    LBG_P1_LEFT /*!< First player's LEFT joystick */,
    LBG_P2_A /*!< Second player's A button */,
    LBG_P2_B /*!< Second player's B button */,
    LBG_P2_UP /*!< Second player's UP joystick */,
    LBG_P2_RIGHT /*!< Second player's RIGHT joystick */,
    LBG_P2_DOWN /*!< Second player's DOWN joystick */,
    LBG_P2_LEFT /*!< Second player's LEFT joystick */,
} lbg_event_t;

/**
 * Errors
 *
 * @see LBG_ERR_STRS
 */
typedef enum lbg_errno_e {
    LBG_ERRNO_GPIO_INIT /*!< Failed to initialize GPIO pins */,
    LBG_ERRNO_GPIO_SETMODE /*!< Failed to set INPUT mode on pin %%d */,
    LBG_ERRNO_LED_INIT /*!< Failed to initialize LEDs */,
    LBG_ERRNO_SUCCESS /*!< Success */
} lbg_errno_t;

/**
 * Error strings
 */
const char *LBG_ERR_STRS[] = {
    "Failed to initialize GPIO pins\n",
    "Failed to set INPUT mode on pin %d\n",
    "Failed to initialize LEDs\n",
    "Success",
};

/**
 * Initialize library and dependencies. Must be called before anything else.
 *
 * @see lbg_exit
 * @see lbg_perror
 * @see lbg_errno
 * @see lbg_errno_details
 *
 * @returns bool
 */
bool lbg_init(void);

/**
 * Terminates library, reset used DMA channels, free up memory, stops running
 * threads. Must be called before program termination.
 * Make sure to hook signal handlers to do cleanup using this function.
 *
 * @see lbg_init
 *
 * @returns none
 */
void lbg_exit(void);

/**
 * Access the 'screen' (LEDs) in a standard 1D matrix and stores it in the first
 * argument.
 * The screen is an array of colors.
 *
 * @see lbg_update_screen
 * @see lbg_errno
 * @see lbg_errno_details
 *
 * @param screen An int32_t allocated array of size LBG_SCREEN_SIZE
 *
 * @returns bool
 */
bool lbg_get_screen(int32_t *screen);

/**
 * Will merge the current screen (displayed on the LEDs) and the new one (passed
 * as parameter) and render the new screen.
 *
 * @see lbg_get_screen
 * @see lbg_errno
 * @see lbg_errno_details
 *
 * @param screen An int32_t array of size LBG_SCREEN_SIZE with desired content.
 *
 * @returns bool
 */
bool lbg_update_screen(int32_t *screen);

/**
 * Poll the next event on the event queue and stores it in the first parameter.
 * Example:
 *
 * @see lbg_event_e
 *
 * @param event A pointer to an lbg_event_t
 *
 * @returns true if there's an available event and false otherwise
 */
bool lbg_poll_event(lbg_event_t *event);

/**
 * Print the last error on stderr.
 *
 * @see lbg_errno
 * @see lbg_errno_details
 *
 * @returns none
 */
void lbg_perror(void);

/** Hold the last error thrown. */
lbg_errno_t lbg_errno = LBG_ERRNO_SUCCESS;
/** Optional details about the last error. Can be an int i or a char array s.*/
union {
    int i;
    char *s;
} lbg_errno_details = {0};

/** @cond NO_DOC */
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

// TODO: add second player
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
