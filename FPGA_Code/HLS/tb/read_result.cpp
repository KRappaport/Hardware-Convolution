#include <stdio.h>
#include <errno.h>
#include "test_bench.h"

int read_result(FILE *fp, float *result){
    // Read the result dimensions
    unsigned short dim[2];
    if (fread(dim, sizeof(short), 2, fp) != 2) {
        perror("Problem reading result dimensions!");
        return(-1);
    }

    // Read the results
    size_t data_size = dim[0]*dim[1];
    if (fread(result, sizeof(float), data_size, fp) != data_size) {
        perror("Problem reading result!");
        return(-1);
    }

    return(data_size);
}
