#include "conv2d.h"


void conv2d(float *img, float ker[DEPTH][KERNEL_DIM*KERNEL_DIM], unsigned short wdth, unsigned int hght, float *img_out){
#pragma HLS INTERFACE axis port=img depth=25 bundle=IMG
#pragma HLS INTERFACE axis port=img_out depth=25 bundle=IMG_OUT
#pragma HLS INTERFACE s_axilite port=return bundle=DIM
#pragma HLS INTERFACE s_axilite port=wdth bundle=DIM
#pragma HLS INTERFACE s_axilite port=hght bundle=DIM
#pragma HLS ARRAY_PARTITION variable=ker complete
#pragma HLS INTERFACE s_axilite port=ker bundle=KER

    float delay_line[KERNEL_DIM-1][MAX_IMG_WIDTH-2];
    float kernel[DEPTH][KERNEL_DIM*KERNEL_DIM], hold[KERNEL_DIM][KERNEL_DIM-1];
    float insert_delay[KERNEL_DIM-1], result;

#pragma HLS ARRAY_PARTITION variable=delay_line complete
#pragma HLS ARRAY_PARTITION variable=hold complete
#pragma HLS ARRAY_PARTITION variable=kernel complete
#pragma HLS ARRAY_PARTITION variable=insert_delay complete

    unsigned int height = hght;
    unsigned short width = wdth;
    unsigned short delay_end = width-3;

    init_delay_line(delay_line);
    init_kernel(ker, kernel);
    init_hold(hold);

    unsigned int row, done;
    unsigned short last_col = width-1;
    unsigned short initial_grbg = (width*(KERNEL_DIM>>1)) + (KERNEL_DIM>>1);
    unsigned short hold_indx1;
    unsigned short hold_indx2;

    for (row = 0; row < height; row++) {
        unsigned short col;
        for (col = 0; col < width; col++) {
            float current_pxl[DEPTH];
            unsigned short depth;
            for (depth = 0; depth < DEPTH; depth++) {
                current_pxl[depth] = *(img++);
            }

            float mult_result[KERNEL_DIM*KERNEL_DIM];
#pragma HLS ARRAY_PARTITION variable=mult_result complete
            unsigned short mult_indx;
            for (mult_indx = 0; mult_indx < (KERNEL_DIM*KERNEL_DIM); mult_indx++) {
                mult_result[mult_indx] = 0;
            }

            for (mult_indx = 0; mult_indx < (KERNEL_DIM*KERNEL_DIM); mult_indx++) {
                for (depth = 0; depth < DEPTH; depth++) {
                    mult_result[mult_indx] += current_pxl[depth]*kernel[depth][mult_indx];
                }
            }

            if (col != 0) {
                result = hold[KERNEL_DIM_1][KERNEL_DIM_2] + mult_result[HIGH_KER_SQR_INDX];
            } else {
                result = hold[KERNEL_DIM_1][KERNEL_DIM_2];
            }
            done = validate_result(initial_grbg, result, img_out);
            mult_indx = HIGH_KER_SQR_INDX-1;
            for (hold_indx2 = KERNEL_DIM_2; hold_indx2 > 0; hold_indx2--) {
                hold[KERNEL_DIM_1][hold_indx2] = hold[KERNEL_DIM_1][hold_indx2-1] + mult_result[mult_indx];
                mult_indx--;
            }
            if (col == last_col) {
                hold[KERNEL_DIM_1][0] = delay_line[KERNEL_DIM_1-1][delay_end];
            } else {
                hold[KERNEL_DIM_1][0] = delay_line[KERNEL_DIM_1-1][delay_end] + mult_result[mult_indx];
            }

            for (hold_indx1 = KERNEL_DIM_2; hold_indx1 > 0; hold_indx1--) {
                mult_indx--;
                if (col != 0) {
                    insert_delay[hold_indx1] = hold[hold_indx1][KERNEL_DIM_2] + mult_result[mult_indx];
                } else {
                    insert_delay[hold_indx1] = hold[hold_indx1][KERNEL_DIM_2];
                }
                advance_delay_line(delay_line, insert_delay[hold_indx1], hold_indx1);
                mult_indx--;
                for (hold_indx2 = KERNEL_DIM_2; hold_indx2 > 0; hold_indx2--) {
                    hold[hold_indx1][hold_indx2] = hold[hold_indx1][hold_indx2-1] + mult_result[mult_indx];
                    mult_indx--;
                }
                if (col == last_col) {
                    hold[hold_indx1][0] = delay_line[hold_indx1-1][delay_end];
                } else {
                    hold[hold_indx1][0] = delay_line[hold_indx1-1][delay_end] + mult_result[mult_indx];
                }
            }

            mult_indx--;
            if (col != 0) {
                insert_delay[0] = hold[0][KERNEL_DIM_2] + mult_result[mult_indx];
            } else {
                insert_delay[0] = hold[0][KERNEL_DIM_2];
            }
            advance_delay_line(delay_line, insert_delay[0], 0);
            mult_indx--;
            for (hold_indx2 = KERNEL_DIM_2; hold_indx2 > 0; hold_indx2--) {
                hold[0][hold_indx2] = hold[0][hold_indx2-1] + mult_result[mult_indx];
                mult_indx--;
            }
            if (col == last_col) {
                hold[0][0] = 0;
            } else {
                hold[0][0] = mult_result[mult_indx];
            }
        }
    }


    for (row = 0; row < initial_grbg; row++) {
        img_out[done] = hold[KERNEL_DIM_1][KERNEL_DIM_2];
        done++;
        unsigned short flush_amnt;
        hold_indx1 = KERNEL_DIM_1;
        for (flush_amnt = KERNEL_DIM_2; flush_amnt >= (KERNEL_DIM>>1); flush_amnt--) {
            for (hold_indx2 = KERNEL_DIM_2; hold_indx2 > 0; hold_indx2--) {
                hold[hold_indx1][hold_indx2] = hold[hold_indx1][hold_indx2-1];
            }
            hold[hold_indx1][0] = delay_line[flush_amnt][delay_end];
            hold_indx1--;
            advance_delay_line(delay_line, hold[hold_indx1][KERNEL_DIM_2], flush_amnt);
        }
        for (hold_indx2 = KERNEL_DIM_2; hold_indx2 > 0; hold_indx2--) {
            hold[hold_indx1][hold_indx2] = hold[hold_indx1][hold_indx2-1];
        }
    }

}
