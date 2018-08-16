#include "data_handling.h"
#include "xparameters.h"

void set_ker(float *kernel, unsigned short dimension, unsigned short depth) {
    float *kernel_base_addr[11*MAX_KER_SIZE];
    int i, k, j=0;

    for (i = 0; i < depth; i++) {
        for (k = 0; k < (dimension*dimension); k++) {
            kernel_base_addr[j++] = (XPAR_BRAM_0_BASEADDR) + (((i*MAX_KER_SIZE)+k)*4);
        }
    }

    for (i = 0; i < (depth*dimension*dimension); i++) {
    	*kernel_base_addr[i] = kernel[i];
    }
}
