#include <errno.h>
#include <signal.h>
#include <string.h>

#include <pigpio.h>

#include "bsargame.h"

bool lbg_init(void) {
    /* Initialize GPIO pins (joysticks & buttons) */
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

    /* Initialize LEDs */
    if (ws2811_init(&LED_STRIP) != WS2811_SUCCESS) {
        lbg_errno = LBG_ERRNO_LED_INIT;
        gpioTerminate();
        return false;
    }

    /* Set up handlers to cleanup */
    struct sigaction sa = {.sa_handler = (void (*)(int))lbg_exit};
    if (sigaction(SIGINT, &sa, NULL) == -1 ||
        sigaction(SIGTERM, &sa, NULL) == -1) {
        lbg_errno = LBG_ERRNO_SIGACTION;
        lbg_errno_details.s = strerror(errno);
        gpioTerminate();
        ws2811_fini(&LED_STRIP);
        return false;
    }

    /* Init done */
    return true;
}

void lbg_exit(void) {
    gpioTerminate();
    ws2811_fini(&LED_STRIP);
}

bool lbg_get_screen(int32_t *screen) {
    (void)screen;
    return true;
}
bool lbg_update_screen(int32_t *screen) {
    (void)screen;
    return true;
}
bool lbg_poll_event(lbg_event_t *event) {
    (void)event;
    return true;
}

void lbg_perror(void) {
    if (lbg_errno >= 0 && lbg_errno <= LBG_ERRNO_SUCCESS) {
        const char *err = LBG_ERR_STRS[lbg_errno];
        if (strstr(err, "%d")) {
            fprintf(stderr, err, lbg_errno_details.i);
        } else if (strstr(err, "%s")) {
            fprintf(stderr, err, lbg_errno_details.s);
        } else {
            fprintf(stderr, "%s", err);
        }
    }
}
