#include "bsargame.h"

int main(void) {
    if (!lbg_init()) {
        lbg_perror();
    }
}
