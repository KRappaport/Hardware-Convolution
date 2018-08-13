#include <stdio.h>
#include "xparameters.h"
#include "xconv2d.h"
#include "xconv2d_hw.h"
#include "xaxidma.h"
#include "inits.h"


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


    return 0;
}
