#ifndef _CONV2D
#define _CONV2D

#ifndef KERNEL_DIM
#define KERNEL_DIM 3
#endif

#ifndef DEPTH
#define DEPTH 3
#endif

#ifndef MAX_KERNEL_DIM_SQR
#define MAX_KERNEL_DIM_SQR 49
#endif

#ifndef MAX_DEPTH
#define MAX_DEPTH 11
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

#include <hls_stream.h>
#include <ap_int.h>
template<int D,int U,int TI,int TD>
  struct ap_axis_fp{
    float            data;
    ap_uint<(D+7)/8> keep;
    ap_uint<(D+7)/8> strb;
    ap_uint<U>       user;
    ap_uint<1>       last;
    ap_uint<TI>      id;
    ap_uint<TD>      dest;
  };

typedef ap_axis_fp <32,1,1,1> AXIS_STRUCT;
typedef hls::stream<AXIS_STRUCT> AXIS_PORT;
typedef hls::stream<float> FP_STREAM;

void conv2d(AXIS_PORT &img, float ker[MAX_DEPTH][MAX_KERNEL_DIM_SQR], unsigned short wdth, unsigned int hght, AXIS_PORT &img_out);

void init_delay_line(FP_STREAM delay_line[KERNEL_DIM_1], unsigned short delay_end);

void init_hold(float hold[KERNEL_DIM][KERNEL_DIM_1]);

void init_kernel(float ker[MAX_DEPTH][MAX_KERNEL_DIM_SQR], float kernel[DEPTH][KERNEL_DIM_SQR]);

void validate_result(unsigned short top_grbg_size, AXIS_STRUCT result, AXIS_PORT &img_out);

void flush_delay_line(FP_STREAM delay_line[KERNEL_DIM_1]);

#endif
