#include <stdio.h>
#include <stdlib.h>
#include "xparameters.h"
#include "xstatus.h"
#include "xaxidma.h"
#include "xil_cache.h"
#include "xtime_l.h"
#include "data_handling.h"


double conv_image(XAxiDma *axidma, float *image, int image_size, float *result, int result_size) {
    int status;
    XTime start, end;

// Flush the data cache to make sure that the data in the RAM is up to date and
// stale data isn't transferred to the FPGA.
    Xil_DCacheFlushRange((unsigned)image, image_size*sizeof(float));
    Xil_DCacheInvalidateRange((unsigned)result, result_size*sizeof(float));

    printf("Convolution starting...\t\t");
    // Start the timer.
    XTime_GetTime(&start);

// Initiate transfer of the image to the FPGA.
    status = XAxiDma_SimpleTransfer(axidma, (u32)image, image_size*sizeof(float), XAXIDMA_DMA_TO_DEVICE);
    if (status != XST_SUCCESS) {
        printf("ERROR: Image transfer failed!\n");
        free(image);
        free(result);
        return(-1.0f);
    }

// Intitate the transfer of the convolution results into RAM
    status = XAxiDma_SimpleTransfer(axidma, (u32)result, result_size*sizeof(float), XAXIDMA_DEVICE_TO_DMA);
    if (status != XST_SUCCESS) {
        printf("ERROR: Result transfer failed!\n");
        free(image);
        free(result);
        return(-1.0f);
    }

    while (XAxiDma_Busy(axidma, XAXIDMA_DEVICE_TO_DMA)) {
        /* Loop here until all of the results have been received. */
    }

    // Stop the timer
    XTime_GetTime(&end);
    printf("Convolution completed!\n");

// Invalidate the data cache for the results so that next time used it will
// ake the fresh data from the RAM.
    Xil_DCacheInvalidateRange((unsigned)result, result_size*sizeof(float));

// Calculate the time for the convolution and return it.  
    double exec_time = (double)(end - start)/COUNTS_PER_SECOND;
    return(exec_time);
}
