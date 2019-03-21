#pragma once

#include <stdint.h>

#define LBG_WIDTH 13
#define LBG_HEIGHT 21
#define LBG_FRAMERATE 10 /* ? */

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
    LBG_P1_C,
    LBG_P1_D,
    LBG_P2_A,
    LBG_P2_B,
    LBG_P2_C,
    LBG_P2_D,
} lbg_event_t;

void lbg_init(void);
void lbg_exit(void);
bool lbg_get_screen(int32_t *screen);
bool lbg_update_screen(int32_t *screen);
bool lbg_poll_event(lbg_event_t *event);
