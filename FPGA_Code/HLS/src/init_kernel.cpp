#include "conv2d.h"


void init_kernel(float ker[MAX_DEPTH][MAX_KERNEL_DIM_SQR], float kernel[DEPTH][KERNEL_DIM_SQR]){
    unsigned short i,k;

// The kernel is stored into a local set of flip flops and is flipped at the
// same time so that the order of the operations occur correctly
    for (k = 0; k < DEPTH; k++) {
        for (i = 0; i < KERNEL_DIM_SQR; i++) {
#pragma HLS pipeline
            kernel[k][HIGH_KER_SQR_INDX-i] = ker[k][i];
        }
    }
}
