#include "conv2d.h"


void init_kernel(float ker[MAX_DEPTH][MAX_KERNEL_DIM_SQR], float kernel[DEPTH][KERNEL_DIM_SQR]){
    unsigned short i,k;
// #pragma HLS pipeline

    for (k = 0; k < DEPTH; k++) {
// #pragma HLS UNROLL
        for (i = 0; i < KERNEL_DIM_SQR; i++) {
#pragma HLS pipeline
// #pragma HLS UNROLL
            kernel[k][HIGH_KER_SQR_INDX-i] = ker[k][i];
        }
    }
}
