#include "conv2d.h"


void advance_delay_line(float delay_line[KERNEL_DIM-1][MAX_IMG_WIDTH-2], float add_delay, unsigned short line_number) {
    unsigned short i;

    for (i = MAX_IMG_WIDTH-3; i > 0; i--) {
        delay_line[line_number][i] = delay_line[line_number][i-1];
    }
    delay_line[line_number][i] = add_delay;
}
