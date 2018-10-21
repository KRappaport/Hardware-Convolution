#include <stdio.h>
#include <errno.h>
#include "test_bench.h"

int read_img(FILE *fp, float *img){
    // Read the image dimensions
    unsigned short dim[3];
    if (fread(dim, sizeof(short), 3, fp) != 3) {
        perror("Problem reading image dimensions!");
        return(-1);
    }

    // Read the image
    size_t data_size = dim[0]*dim[1]*dim[2];
    if (fread(img, sizeof(float), data_size, fp) != data_size) {
        perror("Problem reading image!");
        return(-1);
    }

    return(data_size);
}
