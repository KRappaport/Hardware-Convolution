#include <stdio.h>
#include "xconv2d.h"
#include "inits.h"
#include "xparameters.h"

int xconv2d_init(XConv2d *instptr) {
    XConv2d_Config *cfgptr;
    int status;

    cfgptr = XConv2d_LookupConfig(XPAR_XCONV2D_0_DEVICE_ID);
    if (!cfgptr) {
        printf("ERROR: Lookup of conv2d configuration failed.\n");
        return(XST_FAILURE);
    }

    status = XConv2d_CfgInitialize(instptr, cfgptr);
    if (status != XST_SUCCESS) {
        printf("ERROR: Could not initialize conv2d.\n");
        return(XST_FAILURE);
    }

    return(status);
}
