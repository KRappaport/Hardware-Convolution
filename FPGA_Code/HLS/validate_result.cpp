#include "conv2d.h"


unsigned short top_grbg_processed = 0;

void validate_result(unsigned short top_grbg_size, AXIS_STRUCT result, AXIS_PORT &img_out) {


    if (top_grbg_processed < top_grbg_size) {
        top_grbg_processed++;
    } else {
        img_out.write(result);
    }
}
