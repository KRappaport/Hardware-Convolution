#include <stdio.h>
#include "xparameters.h"
#include "xstatus.h"
#include "xaxidma.h"
#include "xtime_l.h"
#include "data_handling.h"


double conv_image(XAxiDma *axidma, float *image, int image_size, float *result, int result_size) {
    int status;
    XTime start, end;


    Xil_DCacheFlushRange((unsigned)image, image_size*sizeof(float));
    Xil_DCacheInvalidateRange((unsigned)result, result_size*sizeof(float));

    printf("Convolution starting...\t\t");
    XTime_GetTime(&start);

    status = XAxiDma_SimpleTransfer(&axidma, (u32)image, image_size*sizeof(float), XAXIDMA_DMA_TO_DEVICE);
    if (status != XST_SUCCESS) {
        printf("ERROR: Image transfer failed!\n");
        free(image);
        free(result);
        return(-1.0f);
    }

    status = XAxiDma_SimpleTransfer(&axidma, (u32)result, result_size*sizeof(float), XAXIDMA_DEVICE_TO_DMA);
    if (status != XST_SUCCESS) {
        printf("ERROR: Result transfer failed!\n");
        free(image);
        free(result);
        return(-1.0f);
    }

    while (XAxiDma_Busy(&axidma, XAXIDMA_DEVICE_TO_DMA)) {
        /* code */
    }
    XTime_GetTime(&end);
    printf("Convolution completed!\n");

    Xil_DCacheInvalidateRange((unsigned)result, result_size*sizeof(float));

    return((double)((end - start)/COUNTS_PER_SECOND));
}
