#include "data_handling.h"
#include "xparameters.h"


void set_ker(float *kernel, unsigned int kernel_size, unsigned short kernel_dim[3]) {
    float *kernel_base_addr[11*MAX_KER_SIZE];
    int i, k, j=0;

    for (i = 0; i < kernel_dim[2]; i++) {
        for (k = 0; k < (kernel_dim[0]*kernel_dim[1]); k++) {
            kernel_base_addr[j++] = (XPAR_BRAM_0_BASEADDR) + (((i*MAX_KER_SIZE)+k)*4);
        }
    }

    for (i = 0; i < kernel_size; i++) {
    	*kernel_base_addr[i] = kernel[i];
    }
}
