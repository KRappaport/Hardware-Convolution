#include <stdio.h>
#include "xresult_block_4l.h"
#include "inits.h"
#include "xparameters.h"

int xresult_block_4l_init(XResult_block_4l *instptr) {
    XResult_block_4l_Config *cfgptr;
    int status;

    cfgptr = XResult_block_4l_LookupConfig();
    if (!cfgptr) {
        printf("ERROR: Lookup of result block configuration failed.\n");
        return(XST_FAILURE);
    }

    status = XResult_block_4l_CfgInitialize(instptr, cfgptr);
    if (status != XST_SUCCESS) {
        printf("ERROR: Could not initialize result block.\n");
        return(XST_FAILURE);
    }

    return(status);
}
