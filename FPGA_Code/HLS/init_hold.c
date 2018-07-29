#include "conv2d.h"


void init_hold(float hold[KERNEL_DIM][KERNEL_DIM-1]) {
    unsigned short i,k;

    for (i = 0; i < KERNEL_DIM; i++) {
#pragma HLS UNROLL
        for (k = 0; k < KERNEL_DIM_1; k++) {
#pragma HLS UNROLL
            hold[i][k] = 0;
        }
    }
}
