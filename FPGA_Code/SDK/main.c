#include <stdio.h>
#include "xparameters.h"
#include "xconv2d.h"
#include "xconv2d_hw.h"
#include "xil_cache.h"
#include "xaxidma.h"
#include "inits.h"
#include "set_ker.h"


int main() {

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

    unsigned short width = 10;
    XConv2d_Set_wdth(&instptr, width);
    unsigned int height = 10;
    XConv2d_Set_hght(&instptr, height);

    float kernel[3][9];
    set_ker(kernel, 3, 3);

    printf("Starting Conv2D...\n");
    if (XConv2d_IsReady(&instptr)) {
        XConv2d_Start(&instptr);
    } else {
        printf("ERROR: Conv2D not ready!\n");
        exit(-1);
    }
    printf("Started.\n");

    float image[300], result[100];
    Xil_DCacheFlushRange((unsigned)image, 300*sizeof(float));
    Xil_DCacheInvalidateRange((unsigned)result, 100*sizeof(float));

    printf("Initiate image transfer...\n");
    status = XAxiDma_SimpleTransfer(&axidma, (u32)image, 300*sizeof(float), XAXIDMA_DMA_TO_DEVICE);
    if (status != XST_SUCCESS) {
        printf("ERROR: Image transfer failed!\n");
        exit(-1);
    }
    printf("Transferring image\n");

    printf("Initiate result transfer...\n");
    status = XAxiDma_SimpleTransfer(&axidma, (u32)result, 100*sizeof(float), XAXIDMA_DEVICE_TO_DMA);
    if (status != XST_SUCCESS) {
        printf("ERROR: Result transfer failed!\n");
        exit(-1);
    }
    printf("Transferring result\n");

    printf("Waiting for results to finish transferring...\n");
    while (XAxiDma_Busy(&axidma, XAXIDMA_DEVICE_TO_DMA)) {
        /* code */
    }
    printf("Transfer complete\n");

    Xil_DCacheInvalidateRange((unsigned)result, 100*sizeof(float));    

    return 0;
}
