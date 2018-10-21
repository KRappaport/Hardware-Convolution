#include <stdio.h>
#include <stdlib.h>
#include "xparameters.h"
#include "xstatus.h"
#include "xsdps.h"
#include "ff.h"

unsigned short image_dim[3];

int read_image(float **image, TCHAR *filename) {
    FIL fp;
    FRESULT res;

    res = f_open(&fp, filename, FA_READ);
    if (res != FR_OK) {
        printf("ERROR: Failed to open image file! \t Error Code: %d\n", res);
        return(-1);
    }

    unsigned int bytesread, image_size;
    res = f_read(&fp, image_dim, 3*sizeof(short), &bytesread);
    if (res != FR_OK) {
        printf("ERROR: Failed to read image dimensions! \t Error Code: %d\n", res);
        f_close(&fp);
        return(-1);
    } else if (bytesread != (3*sizeof(short))) {
        printf("ERROR: Failed to read image dimensions! \t Bytes Read: %d\n", bytesread);
        f_close(&fp);
        return(-1);
    }

    image_size = image_dim[0]*image_dim[1]*image_dim[2];
    *image = (float*)calloc(image_size, sizeof(float));
    if (*image == NULL) {
        printf("ERROR: Failed to allocate memory for the image!\n");
        f_close(&fp);
        return(-1);
    }

    res = f_read(&fp, *image, image_size*sizeof(float), &bytesread);
    if (res != FR_OK) {
        printf("ERROR: Failed to read image data! \t Error Code: %d\n", res);
        f_close(&fp);
        free(*image);
        *image = NULL;
        return(-1);
    } else if (bytesread != (image_size*sizeof(float))) {
        printf("ERROR: Failed to read image data!\n");
        printf("Bytes Needed: %d \t Bytes Read: %d\n", image_size*sizeof(float), bytesread);
        f_close(&fp);
        free(*image);
        *image = NULL;
        return(-1);
    }

    res = f_close(&fp);
    if (res != FR_OK) {
        printf("ERROR: Failed to close image file!\n");
        free(*image);
        *image = NULL;
        return(-1);
    }

    return(image_size);
}
