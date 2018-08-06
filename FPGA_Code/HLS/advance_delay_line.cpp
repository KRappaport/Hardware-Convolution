#include "conv2d.h"


void advance_delay_line(float delay_line[MAX_IMG_WIDTH], float add_delay, unsigned short delay_end) {
    unsigned short i;

    for (i = 0; i < delay_end; i++) {
        delay_line[i] = delay_line[i+1];
    }
    delay_line[delay_end] = add_delay;
}
