#include "conv2d.h"


void init_hold(hold[KERNEL_DIM][KERNEL_DIM-1]) {
    unsigned short i,k;

    for (i = 0; i < KERNEL_DIM; i++) {
        for (k = 0; k < KERNEL_DIM_1; k++) {
            hold[i][k] = 0;
        }
    }
}
