#include "conv2d.h"


unsigned int validate_result(unsigned short top_grbg_size, float result, float *img_out) {
    static unsigned short top_grbg_processed = 0;
    static unsigned int sent = 0;

    if (top_grbg_processed < top_grbg_size) {
        top_grbg_processed++;
        return(0);
    } else {
        img_out[sent] = result;
        sent++;
        return(sent);
    }
}
