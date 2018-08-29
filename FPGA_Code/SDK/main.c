#include <stdio.h>
#include <stdlib.h>
#include "xparameters.h"
#include "xstatus.h"
#include "xconv2d.h"
#include "xconv2d_hw.h"
#include "xil_cache.h"
#include "xaxidma.h"
#include "xtime_l.h"
#include "xsdps.h"
#include "ff.h"
#include "inits.h"
#include "data_handling.h"

#ifndef TEST
#define TEST
#endif

extern unsigned short kernel_dim[3];
extern unsigned short image_dim[3];

extern int ps7_post_config();


int main() {

    ps7_post_config();

    int status;

    XConv2d instptr;
    status = xconv2d_init(&instptr);
    if (status != XST_SUCCESS) {
        printf("Conv2d setup failed!\n");
        exit(-1);
    }

    XAxiDma axidma;
    status = dma_init(&axidma);
    if (status != XST_SUCCESS) {
        printf("AxiDma setup failed!\n");
        exit(-1);
    }

    FATFS fs;
    status = file_system_init(&fs);
    if (status != XST_SUCCESS) {
        printf("File system setup failed!\n");
        exit(-1);
    }

    int kernel_size;
    float *kernel;
    kernel_size = read_kernel(&kernel, kernel_file);
    if (kernel_size == -1) {
        exit(-1);
    }

    set_ker(kernel, kernel_size, kernel_dim);
    free(kernel);


    int image_size;
    float *image;
    image_size = read_image(&image, image_file);
    if (image_size == -1) {
        exit(-1);
    }

    XConv2d_Set_wdth(&instptr, image_dim[0]);
    XConv2d_Set_hght(&instptr, image_dim[1]);

    printf("Starting Conv2D...\n");
    if (XConv2d_IsReady(&instptr)) {
        XConv2d_Start(&instptr);
    } else {
        printf("ERROR: Conv2D not ready!\n");
        free(image);
        exit(-1);
    }
    printf("Started.\n");

    float *result;
    int result_size = image_dim[0]*image_dim[1];
    result = (float*)calloc(result_size, sizeof(float));
    if (result == NULL) {
        printf("ERROR: Failed to allocate memory for the result!\n");
        free(image);
        exit(-1);
    }

    double conv_exec_time = conv_image(&axidma, image, image_size, result, result_size);
    if (conv_exec_time == -1.0f) {
        printf("ERROR: Transfer of either the image or result failed!\n");
        free(image);
        free(result);
        exit(-1);
    }
    printf("Execution time: %.9f\n", conv_exec_time);

    free(image);

#ifndef TEST
    status = write_result(result, image_dim, result_size, result_fil);
    if (status == -1) {
        free(result);
        exit(-1);
    }
#endif


#ifdef TEST
    float *cmpr_result;
    int cmpr_result_size;
    cmpr_result_size = read_result(&cmpr_result, result_file);
    if (cmpr_result_size == -1) {
        free(result);
        exit(-1);
    } else if (cmpr_result_size != (result_size)) {
        printf("Comparison result file is not the correct size!\n");
        free(result);
        free(cmpr_result);
        exit(-1);
    }

    unsigned int i;
    for (i = 0; i < cmpr_result_size; i++) {
        if (((result[i] - cmpr_result[i]) > 0.005) || ((result[i] - cmpr_result[i]) < -0.005)) {
            printf("FPGA_result[%d]=%f  cmpr_result[%d]=%f\n", i, result[i], i, cmpr_result[i]);
            printf("TEST FAILED!\n");
            free(result);
            free(cmpr_result);
           return(0);
        }
    }
    printf("TEST PASSED!\n");
    free(cmpr_result);
#endif

    
    free(result);

    return 0;
}
