#ifndef _CONV2D
#define _CONV2D

#ifndef MAX_IMG_WIDTH
#define MAX_IMG_WIDTH 5
#endif

#ifndef KERNEL_DIM
#define KERNEL_DIM 3
#endif

#ifndef KERNEL_DIM_SQR
#define KERNEL_DIM_SQR KERNEL_DIM*KERNEL_DIM
#endif

#ifndef DEPTH
#define DEPTH 1
#endif

#ifndef HIGH_KER_SQR_INDX
#define HIGH_KER_SQR_INDX KERNEL_DIM_SQR-1
#endif

#ifndef KERNEL_DIM_1
#define KERNEL_DIM_1 KERNEL_DIM-1
#endif

#ifndef KERNEL_DIM_2
#define KERNEL_DIM_2 KERNEL_DIM-2
#endif

void conv2d(float *img, float ker[DEPTH][KERNEL_DIM*KERNEL_DIM], unsigned short wdth, unsigned int hght, float *img_out);

void init_delay_line(float delay_line[KERNEL_DIM-1][MAX_IMG_WIDTH-2]);

void init_kernel(float ker[DEPTH][KERNEL_DIM_SQR], float kernel[DEPTH][KERNEL_DIM_SQR]);

#endif
