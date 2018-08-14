#ifndef _DATA_HANDLING
#define _DATA_HANDLING

#ifndef MAX_KER_SIZE
#define MAX_KER_SIZE 49
#endif


#include <stdlib.h>
#include "xparameters.h"
#include "xstatus.h"
#include "xsdps.h"
#include "ff.h"

void set_ker(float *kernel[], unsigned short dimension, unsigned short depth);

int read_image(float **image, TCHAR *filename);

#endif
