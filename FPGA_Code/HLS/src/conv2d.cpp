#include <hls_stream.h>
#include <ap_int.h>
#include "conv2d.h"

extern unsigned short top_grbg_processed;


void conv2d(AXIS_PORT &img, float ker[MAX_DEPTH][MAX_KERNEL_DIM_SQR], unsigned short wdth, unsigned int hght, AXIS_PORT &img_out){
#pragma HLS INTERFACE axis port=img bundle=IMG
#pragma HLS INTERFACE axis port=img_out bundle=IMG_OUT
#pragma HLS INTERFACE s_axilite port=return bundle=DIM
#pragma HLS INTERFACE s_axilite port=wdth bundle=DIM
#pragma HLS INTERFACE s_axilite port=hght bundle=DIM
#pragma HLS INTERFACE bram port=ker bundle=KER

    float kernel[DEPTH][KERNEL_DIM_SQR], hold[KERNEL_DIM][KERNEL_DIM_1];
    float insert_delay[KERNEL_DIM_1];

    AXIS_STRUCT result;
    result.strb = 0xF;
    result.keep = 0xF;
    result.last = 0;

#pragma HLS ARRAY_PARTITION variable=hold complete dim=0
#pragma HLS ARRAY_PARTITION variable=kernel complete dim=0
#pragma HLS ARRAY_PARTITION variable=insert_delay complete dim=0

    FP_STREAM delay_line[KERNEL_DIM_1];
#pragma HLS stream variable=delay_line depth=256

// Initializations
    unsigned int height = hght;
    unsigned short width = wdth;
    unsigned short delay_end = width - KERNEL_DIM;

    init_delay_line(delay_line, delay_end);
    init_kernel(ker, kernel);
    init_hold(hold);
    top_grbg_processed = 0;

    unsigned int row, done;
    unsigned short initial_grbg = (width*EDGE_AMOUNT) + EDGE_AMOUNT;
    unsigned short hold_indx1, hold_indx2;
    unsigned short rght_edg_cmp = width - EDGE_AMOUNT;

// Convolution start
    for (row = 0; row < height; row++) {
        unsigned short col;
        for (col = 0; col < width; col++) {
            AXIS_STRUCT current_pxl[DEPTH];
#pragma HLS ARRAY_PARTITION variable=current_pxl complete dim=0
            unsigned short depth;

            float mult_result[KERNEL_DIM_SQR];
#pragma HLS ARRAY_PARTITION variable=mult_result complete dim=0
            unsigned short mult_indx;

// Multiply the current pixel at the zeroth depth with each element of its kernel
            current_pxl[0] = img.read();
            for (mult_indx = 0; mult_indx < KERNEL_DIM_SQR; mult_indx++) {
#pragma HLS UNROLL factor=27
                mult_result[mult_indx] = current_pxl[0].data*kernel[0][mult_indx];
            }

// Multpily the remainder of the depths fro the pixel with their kernel and sum
// the appropriate multiplication results along the depths
            for (depth = 1; depth < DEPTH; depth++) {
#if KERNEL_DIM > 6
#pragma HLS pipeline
#else
#pragma HLS UNROLL
#endif
                current_pxl[depth] = img.read();
                for (mult_indx = 0; mult_indx < KERNEL_DIM_SQR; mult_indx++) {
#pragma HLS UNROLL factor=27
                    mult_result[mult_indx] += current_pxl[depth].data*kernel[depth][mult_indx];
                }
            }

// Zero out incorrect multiplication (for pixels along the edge of the image
// which  shouldn't be multiplied with certain elements of the kernel and thus
// must be zeroed in order that it doesn't affect the results)
            short i,k;
            if (col >= rght_edg_cmp) {   // For the right edge of the image
                for (i = KERNEL_DIM_1; i > (EDGE_AMOUNT + (width-col) - 1); i--) {
                    for (k = 0; k < KERNEL_DIM_SQR; k += KERNEL_DIM) {
#pragma HLS pipeline
#if !((KERNEL_DIM == 7) && (DEPTH > 8)) && !(KERNEL_DIM > 8)
#pragma HLS UNROLL factor=3
#endif
                        mult_result[HIGH_KER_SQR_INDX-i-k] = 0;
                    }
                }
            } else {                     // For the left edge of the image
                for (i = 0; i < (EDGE_AMOUNT-col); i++) {
                    for (k = 0; k < KERNEL_DIM_SQR; k += KERNEL_DIM) {
#pragma HLS pipeline
#if !((KERNEL_DIM == 7) && (DEPTH > 8)) && !(KERNEL_DIM > 8)
#pragma HLS UNROLL factor=3
#endif
                        mult_result[HIGH_KER_SQR_INDX-i-k] = 0;
                    }
                }
            }

// The multiplication results are summed with previous multiplication and
// inserted into a hold or delay line

// The first row of the kernel is handled here.  This row needs to be handled
// differently since it is the end of the line and so there is a possible final
// result to be streamed out instead of inserted into a delay line
            result.data = hold[KERNEL_DIM_1][KERNEL_DIM_2] + mult_result[HIGH_KER_SQR_INDX];
            validate_result(initial_grbg, result, img_out); // Used to check and send result
            mult_indx = HIGH_KER_SQR_INDX-1;
            // Sum and move along elements in the hold line
            for (hold_indx2 = KERNEL_DIM_2; hold_indx2 > 0; hold_indx2--) {
#pragma HLS UNROLL
                hold[KERNEL_DIM_1][hold_indx2] = hold[KERNEL_DIM_1][hold_indx2-1] + mult_result[mult_indx];
                mult_indx--;
            }
            // Sum with first element in the delay line FIFO and move into the hold line
            hold[KERNEL_DIM_1][0] = delay_line[KERNEL_DIM_1-1].read() + mult_result[mult_indx];

// Handling of all of the middle rows of the kernel which have the same set of
// operations
            for (hold_indx1 = KERNEL_DIM_2; hold_indx1 > 0; hold_indx1--) {
#pragma HLS UNROLL
                mult_indx--;
                // Last element of the rows hold line is put into the end of the
                // FIFO delay line.
                insert_delay[hold_indx1] = hold[hold_indx1][KERNEL_DIM_2] + mult_result[mult_indx];
                delay_line[hold_indx1].write(insert_delay[hold_indx1]);
                mult_indx--;
                // Hold elements are summed with proper new multiplications and
                // moved up
                for (hold_indx2 = KERNEL_DIM_2; hold_indx2 > 0; hold_indx2--) {
#pragma HLS UNROLL
                    hold[hold_indx1][hold_indx2] = hold[hold_indx1][hold_indx2-1] + mult_result[mult_indx];
                    mult_indx--;
                }
                // Remove the first element in the delay line FIFO to be summed
                // and placed into the rows first hold spot
                hold[hold_indx1][0] = delay_line[hold_indx1-1].read() + mult_result[mult_indx];
            }

// The last row of the kernel is handled at this point.  It is similar to the
// middle rows except that their is no delay line for it to take anything from
// so the first hold spot is one of the new multiplications not summed with
// anything else
            mult_indx--;
            insert_delay[0] = hold[0][KERNEL_DIM_2] + mult_result[mult_indx];
            delay_line[0].write(insert_delay[0]);
            mult_indx--;
            for (hold_indx2 = KERNEL_DIM_2; hold_indx2 > 0; hold_indx2--) {
#pragma HLS UNROLL
                hold[0][hold_indx2] = hold[0][hold_indx2-1] + mult_result[mult_indx];
                mult_indx--;
            }
            hold[0][0] = mult_result[mult_indx];
        }
    }


// At this point all pixels of the image have been processed, so there are no
// more multiplications or additions to be done.  However, there are still
// results left in hold and delay lines that must be streamed out.
    unsigned short j;
    for (j = KERNEL_DIM_2; j > (KERNEL_DIM_2 - EDGE_AMOUNT); j--) {
#pragma HLS UNROLL
#pragma HLS pipeline
        for (hold_indx2 = KERNEL_DIM_2; hold_indx2 > 0; hold_indx2--) {
#pragma HLS UNROLL
#pragma HLS pipeline
            result.data = hold[j+1][hold_indx2];
            img_out.write(result);
        }
        result.data = hold[j+1][0];
        img_out.write(result);
        while (!delay_line[j].empty()) {
            result.data = delay_line[j].read();
            img_out.write(result);
        }
    }
    for (hold_indx2 = KERNEL_DIM_2; hold_indx2 > (KERNEL_DIM_2 - EDGE_AMOUNT + 1); hold_indx2--) {
#pragma HLS UNROLL
#pragma HLS pipeline
        result.data = hold[j+1][hold_indx2];
        img_out.write(result);
    }
    result.data = hold[j+1][hold_indx2];
    result.last = 1;
    img_out.write(result);

// All results have been sent, bu the delay line is cleaned out before finishing
    flush_delay_line(delay_line);


}
