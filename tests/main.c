#include "bsargame.h"

int main(void) {
    if (!lbg_init()) {
        lbg_perror();
    }

    while (1) {
        lbg_event_t e;
        while (lbg_poll_event(&e)) {
            printf("Got event %d\n", e);
        }
    }
}
