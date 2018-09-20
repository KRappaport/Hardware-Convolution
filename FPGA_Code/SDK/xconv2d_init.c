#include <stdio.h>
#include "xconv2d.h"
#include "inits.h"
#include "xparameters.h"

int xconv_block_init(XConv_block_1 *instptr, unsigned int device_id) {
    XConv_block_1_Config *cfgptr;
    int status;

    cfgptr = XConv_block_1_LookupConfig(device_id);
    if (!cfgptr) {
        printf("ERROR: Lookup of conv2d configuration failed.\n");
        return(XST_FAILURE);
    }

    status = XConv_block_1_CfgInitialize(instptr, cfgptr);
    if (status != XST_SUCCESS) {
        printf("ERROR: Could not initialize conv2d.\n");
        return(XST_FAILURE);
    }

    return(status);
}
