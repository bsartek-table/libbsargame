/** @file */

#include "bsargame.h"

void lbg_get_screen(int32_t *screen) {
    ws2811_led_t *leds = LED_STRIP.channel[0].leds;

    for (int i = 0; i < LBG_SIZE; i++) {
        screen[i] = leds[LED_MAP[i]];
    }
}

// TODO: Framerate
bool lbg_update_screen(int32_t *screen) {
    ws2811_return_t ret;
    ws2811_led_t *leds = LED_STRIP.channel[0].leds;

    for (int i = 0; i < LBG_SIZE; i++) {
        leds[LED_MAP[i]] = screen[i];
    }

    if ((ret = ws2811_render(&LED_STRIP)) != WS2811_SUCCESS) {
        lbg_errno = LBG_ERRNO_LED_RENDER;
        lbg_errno_details.s = (char *)ws2811_get_return_t_str(ret);
        return false;
    }
    return true;
}

bool lbg_clear_screen(void) {
    int32_t screen[LBG_SIZE];

    lbg_get_screen(screen);
    for (int i = 0; i < LBG_SIZE; i++) {
        screen[i] = 0;
    }

    return lbg_update_screen(screen);
}
