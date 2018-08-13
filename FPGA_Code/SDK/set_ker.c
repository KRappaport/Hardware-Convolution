#include "set_ker.h"
#include "xparameters.h"

void set_ker(float *kernel[], unsigned short dimension, unsigned short depth) {
    float *kernel_base_addr = XPAR_BRAM_0_BASEADDR;
    int i, k;

    for (i = 0; i < depth; i++) {
        for (k = 0; k < (dimension*dimension); k++) {
            kernel_base_addr[(i*MAX_KER_SIZE)+k] = kernel[i][k];
        }
    }
}
