#include "bsargame.h"

int main(void) {
    if (!lbg_init()) {
        lbg_perror();
    }

    uint32_t screen[LBG_SIZE] = {0};
    int x = 0;
    int y = 0;
    screen[y * LBG_WIDTH + x] = LBG_GREEN;

    while (1) {
        lbg_event_t e;

        while (lbg_poll_event(&e)) {
            if (e == LBG_P2_UP) {
                printf("salut le haut %d\n", y);
                screen[y * LBG_WIDTH + x] = LBG_BLACK;
                y++;
                if (y > LBG_HEIGHT - 1) {
                    y = LBG_HEIGHT - 1;
                }
            } else if (e == LBG_P2_DOWN) {
                printf("salut le bas %d\n", y);
                screen[y * LBG_WIDTH + x] = LBG_BLACK;
                y--;
                if (y < 0) {
                    y = 0;
                }
            } else {
                printf("no event wtf?\n");
            }
        }

        // printf("%d\n", gpioRead(LBG_GPIO_P2_A));

        screen[y * LBG_WIDTH + x] = LBG_GREEN;
        lbg_update_screen(screen);
    }
}
