#include <stdio.h>
#include "inits.h"
#include "xaxidma.h"
#include "xparameters.h"


int dma_init(XAxiDma *axidma) {
    XAxiDma_Config *cfgptr;
    int status;

    cfgptr = XAxiDma_LookupConfig(XPAR_AXIDMA_0_DEVICE_ID);
    if (!cfgptr) {
        printf("ERROR: Lookup failed in init_dma!\n");
        return(XST_FAILURE);
    }

    status = XAxiDma_CfgInitialize(axidma, cfgptr);
    if (status != XST_SUCCESS) {
        printf("ERROR: Could not initialize in init_dma!\n");
        return(XST_FAILURE);
    }

    if (XAxiDma_HasSg(axidma)) {
        printf("ERROR: DMA in scatter gather mode!\n");
        return(XST_FAILURE);
    }

    XAxiDma_IntrDisable(axidma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);

    return(XST_SUCCESS);
}
