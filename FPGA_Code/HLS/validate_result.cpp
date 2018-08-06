#include "conv2d.h"


void validate_result(unsigned short top_grbg_size, AXIS_STRUCT result, AXIS_PORT &img_out) {
    static unsigned short top_grbg_processed = 0;
    // static unsigned int sent = 0;

    if (top_grbg_processed < top_grbg_size) {
        top_grbg_processed++;
    } else {
        img_out.write(result);
    }
}
