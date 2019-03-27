#include <signal.h>
#include <stdlib.h>

#include "bsargame.h"

static inline int min(int a, int b) { return a < b ? a : b; }
static inline int max(int a, int b) { return a > b ? a : b; }

static void clear_exit(int s) {
    (void)s;
    lbg_clear_screen();
    lbg_render();
    usleep(100000);
    lbg_exit();
}

int main(void) {
    srand(time(NULL));

    if (!lbg_init()) {
        lbg_perror();
    }

    signal(SIGINT, clear_exit);

    int x = 0;
    int y = 0;

    int dx = 0;
    int dy = 0;

    while (1) {
        lbg_event_t e;

        lbg_draw_pixel(x, y, LBG_BLACK);
        lbg_draw_pixel(x - 1, y, LBG_BLACK);

        while (lbg_poll_event(&e)) {
            switch (e) {
            case LBG_P2_PR_UP: dy = 1; break;
            case LBG_P2_PR_DOWN: dy = -1; break;
            case LBG_P2_PR_RIGHT: dx = 1; break;
            case LBG_P2_PR_LEFT: dx = -1; break;
            case LBG_P2_RL_UP:
            case LBG_P2_RL_DOWN: dy = 0; break;
            case LBG_P2_RL_RIGHT:
            case LBG_P2_RL_LEFT: dx = 0; break;
            case LBG_P2_PR_A: lbg_clear_screen(); break;
            }
        }

        x += dx;
        y += dy;

        x = max(0, x);
        x = min(LBG_WIDTH - 1, x);
        y = max(0, y);
        y = min(LBG_HEIGHT - 1, y);

        lbg_draw_pixel(rand() % LBG_WIDTH, rand() % LBG_HEIGHT, LBG_RED);
        lbg_draw_pixel(x, y, LBG_GREEN);
        lbg_draw_pixel(x - 1, y, LBG_GREEN);

        lbg_render();
    }
}
