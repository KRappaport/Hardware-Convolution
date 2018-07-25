#include "conv2d.h"


void init_kernel(float ker[DEPTH][KERNEL_DIM_SQR], float kernel[DEPTH][KERNEL_DIM_SQR]){
    unsigned short i,k;

    for (i = 0; i < KERNEL_DIM_SQR; i++) {
        for (k = 0; k < DEPTH; k++) {
            kernel[k][HIGH_KER_SQR_INDX-i] = ker[k][i];
        }
    }
}
