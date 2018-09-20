#include <hls_stream.h>
#include "conv2d.h"


void init_delay_line(FP_STREAM delay_line[KERNEL_DIM_1], unsigned short delay_end) {
#pragma HLS inline off
    unsigned char dl;
    unsigned short init_push;

// The delay line FIFOs are initialized with the correct amount of zeros, that
// way actual elements are correctly placed at the end of the delay line so that
// they will be accessed at the correct time.  (Zeros are used since they won't
// affect the results since adding a zero has no affect.)
    for (dl = 0; dl < KERNEL_DIM_1; dl++) {
#pragma HLS UNROLL
        for (init_push = 0; init_push <= delay_end; init_push++) {
            delay_line[dl].write(0);
        }
    }
}
