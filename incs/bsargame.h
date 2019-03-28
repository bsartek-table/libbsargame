/** @file */

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <pigpiod_if2.h>
#include <ws2811.h>

/** @cond NO_DOC */
#if defined(DEBUG) || defined(RELEASE)
#else
#define DEBUG 1
#endif
/** @endcond */

#define LBG_WIDTH (13)                    /*!< Screen's width */
#define LBG_HEIGHT (21)                   /*!< Screen's height */
#define LBG_SIZE (LBG_HEIGHT * LBG_WIDTH) /*!< Screen's size */
#define LBG_FRAMERATE (10)                /*!< Screen's framerate */
#define LBG_MAX_EVENTS                                                         \
    (1000) /*!< Maximum number of events stored in the queue */

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

/** Most commonly used colors */
typedef enum lbg_color_e {
    LBG_BLACK = 0x0,            /*!< Black */
    LBG_WHITE = 0x00202020,     /*!< White */
    LBG_RED = 0x00200000,       /*!< Red */
    LBG_YELLOW = 0x00202000,    /*!< Yellow */
    LBG_GREEN = 0x00002000,     /*!< Green */
    LBG_LIGHTBLUE = 0x00002020, /*!< Lightblue */
    LBG_BLUE = 0x00000020,      /*!< Blue */
    LBG_PURPLE = 0x00100010,    /*!< Purple */
    LBG_ORANGE = 0x00201000,    /*!< Orange */
    LBG_PINK = 0x00200010,      /*!< Pink */
} lbg_color_t;

typedef enum lbg_event_e lbg_event_t;

/**
 * Errors
 *
 * @see LBG_ERR_STRS
 */
typedef enum lbg_errno_e {
    LBG_ERRNO_GPIO_INIT,    /*!< Failed to initialize GPIO pins */
    LBG_ERRNO_GPIO_SETMODE, /*!< Failed to set INPUT mode on pin %%d */
    LBG_ERRNO_LED_INIT,     /*!< Failed to initialize LEDs: %s */
    LBG_ERRNO_LED_RENDER,   /*!< Failed to render LEDs: %s */
    LBG_ERRNO_SUCCESS,      /*!< Success */
} lbg_errno_t;

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
 * Terminates library, reset used DMA channels, free up memory, stops
 * running threads. Must be called before program termination. Make sure to
 * hook signal handlers to do cleanup using this function.
 *
 * @see lbg_init
 *
 * @returns none
 */
void lbg_exit(void);

/**
 * Access the 'screen' (LEDs) in a standard 1D matrix and stores it in the
 * first argument. The screen is an array of colors.
 *
 * @see lbg_update_screen
 * @see lbg_errno
 * @see lbg_errno_details
 *
 * @param screen An uint32_t allocated array of size LBG_SCREEN_SIZE
 *
 * @returns none
 */
void lbg_get_screen(uint32_t *screen);

/**
 * Will update the screen with given parameters.
 *
 * @see lbg_get_screen
 * @see lbg_errno
 * @see lbg_errno_details
 *
 * @param screen An uint32_t array of size LBG_SCREEN_SIZE with desired
 * content.
 *
 * @returns none
 */
void lbg_update_screen(uint32_t *screen);

/**
 * Will render the screen at the LBG_FRAMERATE framerate
 *
 * @see lbg_update_screen
 *
 * @returns none
 */
bool lbg_render(void);

/**
 * Will clear out the screen (set all LEDs to LBG_BLACK).
 *
 * @returns none
 */
void lbg_clear_screen(void);

/**
 * Fill the screen with param color
 *
 * @param color
 *
 * @returns none
 */
void lbg_fill_screen(lbg_color_t color);

/**
 * Poll the next event on the event queue and stores it in the first
 * parameter. Example:
 *
 * @see lbg_event_e
 *
 * @param event A pointer to an lbg_event_t
 *
 * @returns true if there's an available event and false otherwise
 */
bool lbg_poll_event(lbg_event_t *event);

/**
 * Clears out the event queue
 *
 * @returns none
 */
void lbg_clear_events(void);

/**
 * Print the last error on stderr.
 *
 * @see lbg_errno
 * @see lbg_errno_details
 *
 * @returns none
 */
void lbg_perror(void);

/**
 * Put a pixel at the given x and y
 *
 * @param x
 * @param y
 *
 * @returns bool
 */
void lbg_draw_pixel(int x, int y, lbg_color_t color);

#define LBG_LED_GPIO_PIN 18 /*!< The GPIO pin used for LEDs */
#define LBG_LED_DMA 10      /*!< The DMA channel used for LEDs */

/** GPIO pins used for buttons and joysticks */
typedef enum lbg_gpio_pin_e {
    LBG_GPIO_P1_A = 27,     /*!< First player's A button */
    LBG_GPIO_P1_B = 17,     /*!< First player's B button */
    LBG_GPIO_P1_UP = 25,    /*!< First player's UP joystick */
    LBG_GPIO_P1_RIGHT = 10, /*!< First player's RIGHT joystick */
    LBG_GPIO_P1_DOWN = 9,   /*!< First player's DOWN joystick */
    LBG_GPIO_P1_LEFT = 11,  /*!< First player's LEFT joystick */
    LBG_GPIO_P2_A = 16,     /*!< Second player's A button */
    LBG_GPIO_P2_B = 20,     /*!< Second player's B button */
    LBG_GPIO_P2_UP = 19,    /*!< Second player's UP joystick */
    LBG_GPIO_P2_RIGHT = 23, /*!< Second player's RIGHT joystick */
    LBG_GPIO_P2_DOWN = 13,  /*!< Second player's DOWN joystick */
    LBG_GPIO_P2_LEFT = 24,  /*!< Second player's LEFT joystick */
} lbg_gpio_pin_t;

/** @cond NO_DOC */
#define LBG_GPIO_LVL_DN (0)
#define LBG_GPIO_LVL_UP (1)

union lbg_errno_details_u {
    int i;
    char *s;
};

extern lbg_errno_t lbg_errno;
extern union lbg_errno_details_u lbg_errno_details;
extern const char *LBG_ERR_STRS[];
extern lbg_gpio_pin_t ALL_PINS[];
extern ws2811_t LED_STRIP;
extern uint32_t LED_MAP[];
extern int LBG_PIGPIOD_IF;
/** @endcond */

/** Available events */
enum lbg_event_e {
    LBG_P1_RL_A = (LBG_GPIO_P1_A << 1) |
                  LBG_GPIO_LVL_DN, /*!< First player's A button released */
    LBG_P1_RL_B = (LBG_GPIO_P1_B << 1) |
                  LBG_GPIO_LVL_DN, /*!< First player's B button released */
    LBG_P1_RL_UP = (LBG_GPIO_P1_UP << 1) |
                   LBG_GPIO_LVL_DN, /*!< First player's UP joystick released */
    LBG_P1_RL_RIGHT =
        (LBG_GPIO_P1_RIGHT << 1) |
        LBG_GPIO_LVL_DN, /*!< First player's RIGHT joystick released */
    LBG_P1_RL_DOWN =
        (LBG_GPIO_P1_DOWN << 1) |
        LBG_GPIO_LVL_DN, /*!< First player's DOWN joystick released */
    LBG_P1_RL_LEFT =
        (LBG_GPIO_P1_LEFT << 1) |
        LBG_GPIO_LVL_DN, /*!< First player's LEFT joystick released */
    LBG_P1_PR_A = (LBG_GPIO_P1_A << 1) |
                  LBG_GPIO_LVL_UP, /*!< First player's A button pressed */
    LBG_P1_PR_B = (LBG_GPIO_P1_B << 1) |
                  LBG_GPIO_LVL_UP, /*!< First player's B button pressed */
    LBG_P1_PR_UP = (LBG_GPIO_P1_UP << 1) |
                   LBG_GPIO_LVL_UP, /*!< First player's UP joystick pressed */
    LBG_P1_PR_RIGHT =
        (LBG_GPIO_P1_RIGHT << 1) |
        LBG_GPIO_LVL_UP, /*!< First player's RIGHT joystick pressed */
    LBG_P1_PR_DOWN =
        (LBG_GPIO_P1_DOWN << 1) |
        LBG_GPIO_LVL_UP, /*!< First player's DOWN joystick pressed */
    LBG_P1_PR_LEFT =
        (LBG_GPIO_P1_LEFT << 1) |
        LBG_GPIO_LVL_UP, /*!< First player's LEFT joystick pressed */
    LBG_P2_RL_A = (LBG_GPIO_P2_A << 1) |
                  LBG_GPIO_LVL_DN, /*!< Second player's A button released */
    LBG_P2_RL_B = (LBG_GPIO_P2_B << 1) |
                  LBG_GPIO_LVL_DN, /*!< Second player's B button released */
    LBG_P2_RL_UP = (LBG_GPIO_P2_UP << 1) |
                   LBG_GPIO_LVL_DN, /*!< Second player's UP joystick released */
    LBG_P2_RL_RIGHT =
        (LBG_GPIO_P2_RIGHT << 1) |
        LBG_GPIO_LVL_DN, /*!< Second player's RIGHT joystick released */
    LBG_P2_RL_DOWN =
        (LBG_GPIO_P2_DOWN << 1) |
        LBG_GPIO_LVL_DN, /*!< Second player's DOWN joystick released */
    LBG_P2_RL_LEFT =
        (LBG_GPIO_P2_LEFT << 1) |
        LBG_GPIO_LVL_DN, /*!< Second player's LEFT joystick released */
    LBG_P2_PR_A = (LBG_GPIO_P2_A << 1) |
                  LBG_GPIO_LVL_UP, /*!< Second player's A button pressed */
    LBG_P2_PR_B = (LBG_GPIO_P2_B << 1) |
                  LBG_GPIO_LVL_UP, /*!< Second player's B button pressed */
    LBG_P2_PR_UP = (LBG_GPIO_P2_UP << 1) |
                   LBG_GPIO_LVL_UP, /*!< Second player's UP joystick pressed */
    LBG_P2_PR_RIGHT =
        (LBG_GPIO_P2_RIGHT << 1) |
        LBG_GPIO_LVL_UP, /*!< Second player's RIGHT joystick pressed */
    LBG_P2_PR_DOWN =
        (LBG_GPIO_P2_DOWN << 1) |
        LBG_GPIO_LVL_UP, /*!< Second player's DOWN joystick pressed */
    LBG_P2_PR_LEFT =
        (LBG_GPIO_P2_LEFT << 1) |
        LBG_GPIO_LVL_UP, /*!< Second player's LEFT joystick pressed */
};
