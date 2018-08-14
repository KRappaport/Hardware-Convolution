#ifndef _INITS
#define _INITS

#include <stdio.h>
#include "xconv2d.h"
#include "xaxidma.h"
#include "xparameters.h"
#include "xsdps.h"
#include "ff.h"
#include "xstatus.h"


int xconv2d_init(XConv2d *instptr);

int dma_init(XAxiDma *axidma);

int file_system_init(FATFS *fs);

#endif
