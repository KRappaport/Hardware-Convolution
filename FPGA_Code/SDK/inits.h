#ifndef _INITS
#define _INITS

#include <stdio.h>
#include "xconv_block_1.h"
#include "xaxidma.h"
#include "xparameters.h"
#include "xsdps.h"
#include "ff.h"
#include "xstatus.h"


int xconv_block_init(XConv_block_1 *instptr, unsigned int device_id);

int dma_init(XAxiDma *axidma);

int file_system_init(FATFS *fs);

#endif
