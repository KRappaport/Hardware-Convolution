#include <hls_stream.h>
#include "conv2d.h"


void init_delay_line(hls::stream<float> delay_line[KERNEL_DIM_1], unsigned short delay_end) {
#pragma HLS inline off
    unsigned char dl;
    unsigned short init_push;

    for (dl = 0; dl < KERNEL_DIM_1; dl++) {
#pragma HLS UNROLL
        for (init_push = 0; init_push <= delay_end; init_push++) {
            delay_line[dl].write(0);
        }
    }
}
