#ifndef _CONV2D
#define _CONV2D

#ifndef KERNEL_DIM
#define KERNEL_DIM 3
#endif

#ifndef DEPTH
#define DEPTH 3
#endif

#ifndef MAX_IMG_WIDTH
#define MAX_IMG_WIDTH 1024
#endif

#ifndef EDGE_AMOUNT
#define EDGE_AMOUNT (KERNEL_DIM>>1)
#endif

#ifndef KERNEL_DIM_SQR
#define KERNEL_DIM_SQR (KERNEL_DIM*KERNEL_DIM)
#endif

#ifndef HIGH_KER_SQR_INDX
#define HIGH_KER_SQR_INDX (KERNEL_DIM_SQR-1)
#endif

#ifndef KERNEL_DIM_1
#define KERNEL_DIM_1 (KERNEL_DIM-1)
#endif

#ifndef KERNEL_DIM_2
#define KERNEL_DIM_2 (KERNEL_DIM-2)
#endif

void conv2d(float *img, float ker[DEPTH][KERNEL_DIM*KERNEL_DIM], unsigned short wdth, unsigned int hght, float *img_out);

void init_delay_line(float delay_line[KERNEL_DIM_1][MAX_IMG_WIDTH], unsigned short delay_end);

void init_hold(float hold[KERNEL_DIM][KERNEL_DIM_1]);

void init_kernel(float ker[DEPTH][KERNEL_DIM_SQR], float kernel[DEPTH][KERNEL_DIM_SQR]);

unsigned int validate_result(unsigned short top_grbg_size, float result, float *img_out);

void advance_delay_line(float delay_line[MAX_IMG_WIDTH], float add_delay, unsigned short delay_end);

#endif
