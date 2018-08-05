#include "conv2d.h"


void init_delay_line(float delay_line[KERNEL_DIM-1][MAX_IMG_WIDTH-2], unsigned short delay_end){
#pragma HLS inline
    unsigned short i,k;

    for (i = 0; i < KERNEL_DIM_1; i++) {
#pragma HLS UNROLL
        for (k = 0; k <= delay_end; k++) {
#pragma HLS UNROLL factor=2 skip_exit_check
            delay_line[i][k] = 0;
        }
    }
}
