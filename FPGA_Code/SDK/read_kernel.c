#include <stdio.h>
#include <stdlib.h>
#include "xparameters.h"
#include "xstatus.h"
#include "xsdps.h"
#include "ff.h"

unsigned short kernel_dim[3];

int read_kernel(float **kernel, TCHAR *filename) {
    FIL *fp;
    FRESULT res;

    res = f_open(fp, filename, FA_READ);
    if (res != FR_OK) {
        printf("ERROR: Failed to open kernel file! \t Error Code: %d\n", res);
        return(-1);
    }

    unsigned int bytesread, kernel_size;
    res = f_read(fp, kernel_dim, 3*sizeof(short), &bytesread);
    if (res != FR_OK) {
        printf("ERROR: Failed to read kernel dimensions! \t Error Code: %d\n", res);
        f_close(fp);
        return(-1);
    } else if (bytesread != (3*sizeof(short))) {
        printf("ERROR: Failed to read kernel dimensions! \t Bytes Read: %d\n", bytesread);
        f_close(fp);
        return(-1);
    }

    kernel_size = kernel_dim[0]*kernel_dim[1]*kernel_dim[2];
    *kernel = (float*)calloc(kernel_size, sizeof(float));
    if (*kernel == NULL) {
        printf("ERROR: Failed to allocate memory for the kernel!\n");
        f_close(fp);
        return(-1);
    }

    res = f_read(fp, *kernel, kernel_size*sizeof(float), &bytesread);
    if (res != FR_OK) {
        printf("ERROR: Failed to read kernel data! \t Error Code: %d\n", res);
        f_close(fp);
        free(*kernel);
        *kernel = NULL;
        return(-1);
    } else if (bytesread != (kernel_size*sizeof(float))) {
        printf("ERROR: Failed to read kernel data!\n");
        printf("Bytes Needed: %d \t Bytes Read: %d\n", kernel_size*sizeof(float), bytesread);
        f_close(fp);
        free(*kernel);
        *kernel = NULL;
        return(-1);
    }

    res = f_close(fp);
    if (res != FR_OK) {
        printf("ERROR: Failed to close kernel file!\n");
        free(*kernel);
        *kernel = NULL;
        return(-1);
    }

    return(kernel_size);
}
