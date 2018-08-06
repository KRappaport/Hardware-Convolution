#include "conv2d.h"


void conv2d(AXIS_PORT &img, float ker[DEPTH][KERNEL_DIM_SQR], unsigned short wdth, unsigned int hght, AXIS_PORT &img_out){
#pragma HLS INTERFACE axis port=img bundle=IMG
#pragma HLS INTERFACE axis port=img_out bundle=IMG_OUT
#pragma HLS INTERFACE s_axilite port=return bundle=DIM
#pragma HLS INTERFACE s_axilite port=wdth bundle=DIM
#pragma HLS INTERFACE s_axilite port=hght bundle=DIM
#pragma HLS INTERFACE s_axilite port=ker bundle=KER

    float delay_line[KERNEL_DIM_1][MAX_IMG_WIDTH];
    float kernel[DEPTH][KERNEL_DIM_SQR], hold[KERNEL_DIM][KERNEL_DIM_1];
    float insert_delay[KERNEL_DIM_1];
    AXIS_STRUCT result;
    result.strb = 0xF;
    result.keep = 0xF;
    result.last = 0;

#pragma HLS ARRAY_PARTITION variable=delay_line complete dim=1
#pragma HLS resource variable=delay_line core=RAM_T2P_BRAM
#pragma HLS ARRAY_PARTITION variable=hold complete dim=0
#pragma HLS ARRAY_PARTITION variable=kernel complete dim=0
#pragma HLS ARRAY_PARTITION variable=insert_delay complete dim=0

    unsigned int height = hght;
    unsigned short width = wdth;
    unsigned short delay_end = width - KERNEL_DIM;

    init_delay_line(delay_line, delay_end);
    init_kernel(ker, kernel);
    init_hold(hold);

    unsigned int row, done;
    unsigned short initial_grbg = (width*EDGE_AMOUNT) + EDGE_AMOUNT;
    unsigned short hold_indx1;
    unsigned short hold_indx2;
    unsigned short rght_edg_cmp = width - EDGE_AMOUNT;

    for (row = 0; row < height; row++) {
        unsigned short col;
        for (col = 0; col < width; col++) {
            AXIS_STRUCT current_pxl[DEPTH];
#pragma HLS ARRAY_PARTITION variable=current_pxl complete dim=0
            unsigned short depth;

            float mult_result[KERNEL_DIM_SQR];
#pragma HLS ARRAY_PARTITION variable=mult_result complete dim=0
            unsigned short mult_indx;

            current_pxl[0] = img.read();
            for (mult_indx = 0; mult_indx < KERNEL_DIM_SQR; mult_indx++) {
#pragma HLS UNROLL factor=25
                mult_result[mult_indx] = current_pxl[0].data*kernel[0][mult_indx];
            }


            for (depth = 1; depth < DEPTH; depth++) {
#pragma HLS UNROLL
                current_pxl[depth] = img.read();
                for (mult_indx = 0; mult_indx < KERNEL_DIM_SQR; mult_indx++) {
#pragma HLS UNROLL factor=25
                    mult_result[mult_indx] += current_pxl[depth].data*kernel[depth][mult_indx];
                }
            }

//             for (depth = 1; depth < DEPTH; depth++) {
// // #pragma HLS UNROLL
//                 for (mult_indx = 0; mult_indx < (KERNEL_DIM*KERNEL_DIM); mult_indx++) {
// #pragma HLS UNROLL factor=22
//                     mult_result[0][mult_indx] = mult_result[0][mult_indx] + mult_result[depth][mult_indx];
//                 }
//             }

            short i,k;
            if (col >= rght_edg_cmp) {
                for (i = KERNEL_DIM_1; i > (EDGE_AMOUNT + (width-col) - 1); i--) {
                    for (k = 0; k < KERNEL_DIM_SQR; k += KERNEL_DIM) {
#pragma HLS pipeline
#pragma HLS UNROLL factor=3
                        mult_result[HIGH_KER_SQR_INDX-i-k] = 0;
                    }
                }
            } else {
                for (i = 0; i < (EDGE_AMOUNT-col); i++) {
                    for (k = 0; k < KERNEL_DIM_SQR; k += KERNEL_DIM) {
#pragma HLS pipeline
#pragma HLS UNROLL factor=3
                        mult_result[HIGH_KER_SQR_INDX-i-k] = 0;
                    }
                }
            }


            result.data = hold[KERNEL_DIM_1][KERNEL_DIM_2] + mult_result[HIGH_KER_SQR_INDX];
            validate_result(initial_grbg, result, img_out);
            mult_indx = HIGH_KER_SQR_INDX-1;
            for (hold_indx2 = KERNEL_DIM_2; hold_indx2 > 0; hold_indx2--) {
#pragma HLS UNROLL
                hold[KERNEL_DIM_1][hold_indx2] = hold[KERNEL_DIM_1][hold_indx2-1] + mult_result[mult_indx];
                mult_indx--;
            }
            hold[KERNEL_DIM_1][0] = delay_line[KERNEL_DIM_1-1][0] + mult_result[mult_indx];

            for (hold_indx1 = KERNEL_DIM_2; hold_indx1 > 0; hold_indx1--) {
#pragma HLS UNROLL
                mult_indx--;
                insert_delay[hold_indx1] = hold[hold_indx1][KERNEL_DIM_2] + mult_result[mult_indx];
                advance_delay_line(delay_line[hold_indx1], insert_delay[hold_indx1], delay_end);
                mult_indx--;
                for (hold_indx2 = KERNEL_DIM_2; hold_indx2 > 0; hold_indx2--) {
#pragma HLS UNROLL
                    hold[hold_indx1][hold_indx2] = hold[hold_indx1][hold_indx2-1] + mult_result[mult_indx];
                    mult_indx--;
                }
                hold[hold_indx1][0] = delay_line[hold_indx1-1][0] + mult_result[mult_indx];
            }

            mult_indx--;
            insert_delay[0] = hold[0][KERNEL_DIM_2] + mult_result[mult_indx];
            advance_delay_line(delay_line[0], insert_delay[0], delay_end);
            mult_indx--;
            for (hold_indx2 = KERNEL_DIM_2; hold_indx2 > 0; hold_indx2--) {
#pragma HLS UNROLL
                hold[0][hold_indx2] = hold[0][hold_indx2-1] + mult_result[mult_indx];
                mult_indx--;
            }
            hold[0][0] = mult_result[mult_indx];
        }
    }


    for (row = 0; row < initial_grbg; row++) {
        result.data = hold[KERNEL_DIM_1][KERNEL_DIM_2];
        if (row == (initial_grbg-1)) {
            result.last = 1;
        }
        img_out.write(result);
        unsigned short flush_amnt;
        hold_indx1 = KERNEL_DIM_1;
        for (flush_amnt = KERNEL_DIM_2; flush_amnt >= EDGE_AMOUNT; flush_amnt--) {
#pragma HLS UNROLL
            for (hold_indx2 = KERNEL_DIM_2; hold_indx2 > 0; hold_indx2--) {
#pragma HLS UNROLL
                hold[hold_indx1][hold_indx2] = hold[hold_indx1][hold_indx2-1];
            }
            hold[hold_indx1][0] = delay_line[flush_amnt][0];
            hold_indx1--;
            advance_delay_line(delay_line[flush_amnt], hold[hold_indx1][KERNEL_DIM_2], delay_end);
        }
        for (hold_indx2 = KERNEL_DIM_2; hold_indx2 > 0; hold_indx2--) {
#pragma HLS UNROLL
            hold[hold_indx1][hold_indx2] = hold[hold_indx1][hold_indx2-1];
        }
    }

}
