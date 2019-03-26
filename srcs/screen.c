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

// TODO: Framerate?
bool lbg_update_screen(int32_t *screen) {
    ws2811_return_t ret;
    ws2811_led_t *leds = LED_STRIP.channel[0].leds;

    for (int x = 0; x < LBG_WIDTH; x++) {
        for (int y = 0; y < LBG_HEIGHT; y++) {
            leds[y * LBG_WIDTH + x] = screen[y * LBG_WIDTH + x];
        }
    }

    if ((ret = ws2811_render(&LED_STRIP)) != WS2811_SUCCESS) {
        lbg_errno = LBG_ERRNO_LED_RENDER;
        lbg_errno_details.s = ws2811_get_return_t_str(ret);
        return false;
    }
    return true;
}

bool lbg_clear_screen(void) {
    ws2811_return_t ret;

    for (int i = 0; i < LBG_SIZE; i++) {
        LED_STRIP.channel[0].leds[i] = 0;
    }

    if ((ret = ws2811_render(&LED_STRIP)) != WS2811_SUCCESS) {
        lbg_errno = LBG_ERRNO_LED_RENDER;
        lbg_errno_details.s = ws2811_get_return_t_str(ret);
        return false;
    }
    return true;
}
