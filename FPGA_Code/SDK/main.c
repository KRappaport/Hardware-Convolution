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
    kernel_size = read_kernel(&kernel, *kernel_file);
    if (kernel_size == -1) {
        exit(-1);
    }

    set_ker(kernel, kernel_size, kernel_dim);
    free(kernel);

    TCHAR *image_file;
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
    result = (float*)calloc(image_dim[0]*image_dim[1], sizeof(float));
    if (result == NULL) {
        printf("ERROR: Failed to allocate memory for the result!\n");
        free(image);
        exit(-1);
    }

    XTime start, end;
    XTime_GetTime(&start);


    Xil_DCacheFlushRange((unsigned)image, image_size*sizeof(float));
    Xil_DCacheInvalidateRange((unsigned)result, image_dim[0]*image_dim[1]*sizeof(float));

    printf("Initiate image transfer...\n");
    status = XAxiDma_SimpleTransfer(&axidma, (u32)image, image_size*sizeof(float), XAXIDMA_DMA_TO_DEVICE);
    if (status != XST_SUCCESS) {
        printf("ERROR: Image transfer failed!\n");
        free(image);
        free(result);
        exit(-1);
    }
    printf("Transferring image\n");

    printf("Initiate result transfer...\n");
    status = XAxiDma_SimpleTransfer(&axidma, (u32)result, image_dim[0]*image_dim[1]*sizeof(float), XAXIDMA_DEVICE_TO_DMA);
    if (status != XST_SUCCESS) {
        printf("ERROR: Result transfer failed!\n");
        free(image);
        free(result);
        exit(-1);
    }
    printf("Transferring result\n");

    printf("Waiting for results to finish transferring...\n");
    while (XAxiDma_Busy(&axidma, XAXIDMA_DEVICE_TO_DMA)) {
        /* code */
    }
    printf("Transfer complete\n");

    Xil_DCacheInvalidateRange((unsigned)result, image_dim[0]*image_dim[1]*sizeof(float));

    XTime_GetTime(&end);

    double conv_exec_time;
    conv_exec_time = (double)(end - start)/COUNTS_PER_SECOND;
    printf("Execution time: %f\n", conv_exec_time);

    free(image);

    float *cmpr_result;
    int cmpr_result_size;
    cmpr_result_size = read_result(&cmpr_result, result_file);
    if (cmpr_result_size == -1) {
        free(result);
        exit(-1);
    } else if (cmpr_result_size != (image_dim[0]*image_dim[1])) {
        printf("Comparison result file is not the correct size!\n");
        free(result);
        free(cmpr_result);
        exit(-1);
    }

    return 0;
}
