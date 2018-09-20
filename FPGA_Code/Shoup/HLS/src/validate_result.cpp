#include "conv2d.h"


unsigned short top_grbg_processed = 0;

void validate_result(unsigned short top_grbg_size, AXIS_STRUCT result, AXIS_PORT &img_out) {

// Here possible results are checked to see if it should be sent out or
// disregarded as initial garbage that isn't actually part of the results
    if (top_grbg_processed < top_grbg_size) {
        top_grbg_processed++;
    } else {
        img_out.write(result);
    }
}
