#include <stdio.h>
#include <stdlib.h>
#include "xparameters.h"
#include "xstatus.h"
#include "xsdps.h"
#include "ff.h"

int read_result(float **cmpr_result, TCHAR *filename) {
    FIL fp;
    FRESULT res;

    res = f_open(&fp, filename, FA_READ);
    if (res != FR_OK) {
        printf("ERROR: Failed to open result file! \t Error Code: %d\n", res);
        return(-1);
    }

    unsigned int bytesread, result_size;
    unsigned short result_dim[2];
    res = f_read(&fp, result_dim, 2*sizeof(short), &bytesread);
    if (res != FR_OK) {
        printf("ERROR: Failed to read result dimensions! \t Error Code: %d\n", res);
        f_close(&fp);
        return(-1);
    } else if (bytesread != (2*sizeof(short))) {
        printf("ERROR: Failed to read result dimensions! \t Bytes Read: %d\n", bytesread);
        f_close(&fp);
        return(-1);
    }

    result_size = result_dim[0]*result_dim[1];
    *cmpr_result = (float*)calloc(result_size, sizeof(float));
    if (*result == NULL) {
        printf("ERROR: Failed to allocate memory for the result!\n");
        f_close(fp);
        return(-1);
    }

    res = f_read(&fp, *cmpr_result, result_size*sizeof(float), &bytesread);
    if (res != FR_OK) {
        printf("ERROR: Failed to read result data! \t Error Code: %d\n", res);
        f_close(&fp);
        free(*cmpr_result);
        *cmpr_result = NULL;
        return(-1);
    } else if (bytesread != (result_size*sizeof(float))) {
        printf("ERROR: Failed to read result data!\n");
        printf("Bytes Needed: %d \t Bytes Read: %d\n", result_size*sizeof(float), bytesread);
        f_close(&fp);
        free(*cmpr_result);
        *cmpr_result = NULL;
        return(-1);
    }

    res = f_close(&fp);
    if (res != FR_OK) {
        printf("ERROR: Failed to close result file!\n");
        free(*cmpr_result);
        *cmpr_result = NULL;
        return(-1);
    }

    return(result_size);
}
