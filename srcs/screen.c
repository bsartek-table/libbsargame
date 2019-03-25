/** @file */

#include "bsargame.h"

bool lbg_get_screen(int32_t *screen) {
    ws2811_led_t *leds = LED_STRIP.channel[0].leds;

    for (int y = 0; y < (LBG_HEIGHT - 1); y++) {
        for (int x = 0; x < LBG_WIDTH; x++) {
            uint32_t l = leds[(y + 1) * LBG_WIDTH + LBG_WIDTH - x - 1];
            screen[y * LBG_WIDTH + x] = l;
        }
    }
    return true;
}

bool lbg_update_screen(int32_t *screen) {
    ws2811_led_t *leds = LED_STRIP.channel[0].leds;

    for (int x = 0; x < LBG_WIDTH; x++) {
        for (int y = 0; y < LBG_HEIGHT; y++) {
            leds[y * LBG_WIDTH + x] = screen[y * LBG_WIDTH + x];
        }
    }

    if (ws2811_render(&LED_STRIP) != WS2811_SUCCESS) {
        // TODO: set errno
        return false;
    }
    return true;
}
