#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include "bsargame.h"

static inline int min(int a, int b) { return a < b ? a : b; }
static inline int max(int a, int b) { return a > b ? a : b; }

typedef struct pos_s {
    int x;
    int y;
} pos_t;

typedef struct snake_s {
    pos_t px[LBG_SIZE];
    int size;
    pos_t food;
} snake_t;

static int mod(int a, int b) {
    int r = a % b;
    return r < 0 ? r + b : r;
}

static void clear_exit(__attribute__((unused)) int s) {
    lbg_clear_screen();
    lbg_render();
    sleep(1);
    lbg_exit();
}

static void gen_food(snake_t *snake) {
    pos_t head = snake->px[0];

    do {
        snake->food.x = rand() % LBG_WIDTH;
        snake->food.y = rand() % LBG_HEIGHT;
    } while (head.x == snake->food.x && head.y == snake->food.y);
}

static void draw_snake(snake_t *snake) {
    for (int i = 0; i < snake->size; i++) {
        lbg_draw_pixel(snake->px[i].x, snake->px[i].y, LBG_GREEN);
    }
    lbg_draw_pixel(snake->food.x, snake->food.y, LBG_RED);
}

static bool move_snake(snake_t *snake, int dx, int dy) {
    pos_t head = snake->px[0];
    pos_t new_head = {.x = mod(snake->px[0].x + dx, LBG_WIDTH),
                      .y = mod(snake->px[0].y + dy, LBG_HEIGHT)};

    // Collision
    for (int i = 1; i < snake->size; i++) {
        pos_t px = snake->px[i];
        if (i != snake->size - 1 && head.x == snake->px[i].x &&
            head.y == snake->px[i].y) {
            return false;
        }
    }

    // Food
    if (head.x == snake->food.x && head.y == snake->food.y) {
        gen_food(snake);
        snake->size++;
    }

    // Next px
    for (int i = snake->size - 2; i >= 0; i--) {
        snake->px[i + 1] = snake->px[i];
    }
    snake->px[0] = new_head;

    return true;
}

int main(void) {
    srand(time(NULL));

    if (!lbg_init()) {
        lbg_perror();
        exit(1);
    }
    signal(SIGINT, clear_exit);

    while (1) {
        snake_t snake = {.px = {{0, 2}, {0, 1}, {0, 0}},
                         .size = 3,
                         .food = {rand() % LBG_WIDTH, rand() % LBG_HEIGHT}};

        int dx = 0, dy = 1;

        while (1) {
            lbg_event_t e;
            while (lbg_poll_event(&e)) {
                switch (e) {
                case LBG_P2_PR_UP:
                    dy = dy == 0 ? 1 : dy;
                    dx = 0;
                    break;
                case LBG_P2_PR_DOWN:
                    dy = dy == 0 ? -1 : dy;
                    dx = 0;
                    break;
                case LBG_P2_PR_RIGHT:
                    dx = dx == 0 ? 1 : dx;
                    dy = 0;
                    break;
                case LBG_P2_PR_LEFT:
                    dx = dx == 0 ? -1 : dx;
                    dy = 0;
                    break;
                }
            }

            if (!move_snake(&snake, dx, dy)) {
                break;
            }
            lbg_clear_screen();
            draw_snake(&snake);
            lbg_render();
        }

        for (int i = 0; i < 10; i++) {
            lbg_fill_screen(LBG_RED);
            lbg_render();
            lbg_clear_screen();
            lbg_render();
        }
    }
}
