/** @file */

#include <time.h>

#include "bsargame.h"

static struct timespec start_ts;
static uint32_t last_updated = 0;
static const int ms_interval = 1000 / LBG_FRAMERATE;

static uint32_t get_ticks(void) {
    uint32_t ticks;
    static bool has_init_ticks = false;

    if (!has_init_ticks) {
        clock_gettime(CLOCK_MONOTONIC_RAW, &start_ts);
        has_init_ticks = true;
    }

    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC_RAW, &now);
    ticks = (uint32_t)((now.tv_sec - start_ts.tv_sec) * 1000 +
                       (now.tv_nsec - start_ts.tv_nsec) / 1000000);
    return ticks;
}

static void delay(uint32_t ms) {
    int e;
    struct timespec elapsed, tv;

    elapsed.tv_sec = ms / 1000;
    elapsed.tv_nsec = (ms % 1000) * 1000000;

    do {

        tv.tv_sec = elapsed.tv_sec;
        tv.tv_nsec = elapsed.tv_nsec;
        e = nanosleep(&tv, &elapsed);
    } while (e);
}

void lbg_get_screen(uint32_t *screen) {
    ws2811_led_t *leds = LED_STRIP.channel[0].leds;

    for (int i = 0; i < LBG_SIZE; i++) {
        screen[i] = leds[LED_MAP[i]];
    }
}

void lbg_update_screen(uint32_t *screen) {
    for (int i = 0; i < LBG_SIZE; i++) {
        LED_STRIP.channel[0].leds[LED_MAP[i]] = screen[i];
    }
}

void lbg_clear_screen(void) {
    uint32_t screen[LBG_SIZE] = {0};

    lbg_update_screen(screen);
}

bool lbg_render(void) {
    ws2811_return_t ret;

    int32_t now = get_ticks();
    int32_t d = ms_interval - (now - last_updated);
    delay(d);

    if ((ret = ws2811_render(&LED_STRIP)) != WS2811_SUCCESS) {
        lbg_errno = LBG_ERRNO_LED_RENDER;
        lbg_errno_details.s = (char *)ws2811_get_return_t_str(ret);
        return false;
    }

    last_updated = get_ticks();
    return true;
}
