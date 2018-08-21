#ifndef _DATA_HANDLING
#define _DATA_HANDLING

#ifndef MAX_KER_SIZE
#define MAX_KER_SIZE 49
#endif


#include <stdio.h>
#include <stdlib.h>
#include "xparameters.h"
#include "xstatus.h"
#include "xsdps.h"
#include "ff.h"

void set_ker(float *kernel, unsigned int kernel_size, unsigned short kernel_dim[3]);

int read_image(float **image, TCHAR *filename);

int read_kernel(float **kernel, TCHAR *filename);

int read_result(float **cmpr_result, TCHAR *filename);

#endif
