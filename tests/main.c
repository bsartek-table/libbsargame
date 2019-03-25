#include "bsargame.h"

int main(void) {
    if (!lbg_init()) {
        lbg_perror();
    }

    uint32_t screen[LBG_SIZE];
    for (int i = 0; i < LBG_SIZE; i++) {
        printf("%d ", screen[i]);
    }
    printf("\n");

    while (1) {
        lbg_event_t e;

        if (!lbg_get_screen(screen)) {
            lbg_debug("Fatal error on get_screen");
        }

        screen[0] = LBG_RED;
        screen[4] = LBG_YELLOW;
        screen[14] = LBG_GREEN;
        screen[LBG_SIZE - 1] = LBG_BLUE;
        lbg_update_screen(screen);
    }
}
