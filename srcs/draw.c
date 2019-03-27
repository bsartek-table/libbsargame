/** @file */

#include "bsargame.h"

static inline int rd(int x, int y) { return y * LBG_WIDTH + x; }

void lbg_draw_pixel(int x, int y, lbg_color_t color) {
    uint32_t screen[LBG_SIZE];

    lbg_get_screen(screen);
    screen[rd(x, y)] = color;
    lbg_update_screen(screen);
}
