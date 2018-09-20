#include <stdio.h>
#include "xparameters.h"
#include "xstatus.h"
#include "xsdps.h"
#include "ff.h"
#include "data_handling.h"

int write_result(float *result, unsigned short *result_dim, int result_size, TCHAR *filename) {
    FIL fp;
    FRESULT res;

    res = f_open(&fp, filename, FA_CREATE_ALWAYS | FA_WRITE);
    if (res != FR_OK) {
        printf("ERROR: Failed to open result file! \t Error Code: %d\n", res);
        return(-1);
    }

    unsigned int byteswrote;
    res = f_write(&fp, result_dim, 2*sizeof(short), &byteswrote);
    if (res != FR_OK) {
        printf("ERROR: Failed to write result dimensions! \t Error Code: %d\n", res);
        f_close(&fp);
        return(-1);
    } else if (byteswrote != (2*sizeof(short))) {
        printf("ERROR: Failed to write result dimensions! \t Bytes Written: %d\n", byteswrote);
        f_close(&fp);
        return(-1);
    }

    res = f_write(&fp, result, result_size*sizeof(float), &byteswrote);
    if (res != FR_OK) {
        printf("ERROR: Failed to write result! \t Error Code: %d\n", res);
        f_close(&fp);
        return(-1);
    } else if (byteswrote != (result_size*sizeof(float))) {
        printf("ERROR: Failed to write result! \t Bytes Written: %d\n", byteswrote);
        f_close(&fp);
        return(-1);
    }

    res = f_close(&fp);
    if (res != FR_OK) {
        printf("ERROR: Failed to close result file!\n");
        return(-1);
    }

    return(byteswrote);
}
