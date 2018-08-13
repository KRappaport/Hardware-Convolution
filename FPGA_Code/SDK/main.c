#include <stdio.h>
#include "xparameters.h"
#include "xconv2d.h"
#include "xconv2d_hw.h"
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

    return 0;
}
