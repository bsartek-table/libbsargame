/** @file */

#include "bsargame.h"

void lbg_perror(void) {
    if (lbg_errno >= 0 && lbg_errno <= LBG_ERRNO_SUCCESS) {
        const char *err = LBG_ERR_STRS[lbg_errno];
        if (strstr(err, "%d")) {
            fprintf(stderr, err, lbg_errno_details.i);
        } else if (strstr(err, "%s")) {
            fprintf(stderr, err, lbg_errno_details.s);
        } else {
            fprintf(stderr, "%s", err);
        }
    }
}
