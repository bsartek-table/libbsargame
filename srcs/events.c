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
        if (eventQ.rear < LBG_MAX_EVENTS) {
            eventQ.q[++eventQ.rear] = evt;
            eventQ.count++;
        }
    }
}

static lbg_event_t pop_event(void) {
    eventQ.count--;
    return eventQ.q[eventQ.front++];
}

// TODO: Keep emitting event until level changes
static void gpioAlertF(int gpio, __attribute__((unused)) int level,
                       __attribute__((unused)) uint32_t tick) {
    int i;

    if (level == 0) {
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
        if (gpioSetAlertFunc(ALL_PINS[i], gpioAlertF) != 0) {
            return false;
        }
    }
    *first_run = false;
    return true;
}

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
