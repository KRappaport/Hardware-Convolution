#include <hls_stream.h>
#include "conv2d.h"


void flush_delay_line(hls::stream<float> delay_line[KERNEL_DIM_1]) {
#pragma HLS DATAFLOW
    unsigned char dl;

    for (dl = 0; dl < KERNEL_DIM_1; dl++) {
#pragma HLS UNROLL
        while (!delay_line[dl].empty()) {
            delay_line[dl].read();
        }
    }
}
