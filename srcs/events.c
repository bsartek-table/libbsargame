/** @file */

#include "bsargame.h"

static struct {
    int front;
    int rear;
    int count;
    lbg_event_t q[LBG_MAX_EVENTS];
} eventQ = {0, -1, 0, {-1}};

static void push_event(lbg_event_t evt) {
    if (eventQ.count < LBG_MAX_EVENTS) {
        if (eventQ.rear == LBG_MAX_EVENTS - 1) {
            eventQ.rear = -1;
        }

        eventQ.q[++eventQ.rear] = evt;
        eventQ.count++;
    }
}

static lbg_event_t pop_event(void) {
    lbg_event_t evt = eventQ.q[eventQ.front++];

    if (eventQ.front == LBG_MAX_EVENTS) {
        eventQ.front = 0;
    }

    eventQ.count--;
    return evt;
}

static void gpioAlertF(int gpio, int level, uint32_t tick) {
    int i;

    printf("gpio=%d lvl=%d tick=%u\n", gpio, level, tick);

    if (level != 1) {
        return;
    }

    for (i = 0; ALL_PINS[i] != (lbg_gpio_pin_t)-1; i++) {
        if (ALL_PINS[i] == (lbg_gpio_pin_t)gpio) {
            break;
        }
    }

    if (ALL_EVENTS[i] != (lbg_event_t)-1) {
        push_event(ALL_EVENTS[i]);
    }
}

static bool do_first_run(bool *first_run) {
    for (int i = 0; ALL_PINS[i] != (lbg_gpio_pin_t)-1; i++) {
        // TODO: Test with gpioSetISRFunc?
        if (gpioSetAlertFunc(ALL_PINS[i], gpioAlertF) != 0) {
            return false;
        }
    }
    *first_run = false;
    return true;
}

// TODO:
// Fix the poll_event: x_release and x_pressed events + dont use alertFunc
// (should test setISRFunc)
// epoll on gpioNotify handle?
bool lbg_poll_event(lbg_event_t *event) {
    static bool first_run = true;

    if (first_run) {
        if (!do_first_run(&first_run)) {
            return false;
        }
    }

    if (eventQ.count == 0) {
        return false;
    }

    *event = pop_event();
    return true;
}
