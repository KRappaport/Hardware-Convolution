#include "conv2d.h"


void init_hold(float hold[KERNEL_DIM][KERNEL_DIM_1]) {
    unsigned short i,k;

// The hold flip flops are zeroed out so that old and random values stored in
// them won't affect the convolution for the new image
    for (i = 0; i < KERNEL_DIM; i++) {
#pragma HLS UNROLL
        for (k = 0; k < KERNEL_DIM_1; k++) {
#pragma HLS UNROLL
            hold[i][k] = 0;
        }
    }
}
