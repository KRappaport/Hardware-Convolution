#ifndef _TEST_BENCH
#define _TEST_BENCH

#ifndef KERNEL_FILE
#define KERNEL_FILE "filename"  //Replace filename with the file containing the desired kernel
#endif

#ifndef IMAGE_FILE
#define IMAGE_FILE "filename"  //Replace filename with the file containing the desired image
#endif

#ifndef IMAGE_HEIGHT
#define IMAGE_HEIGHT 10
#endif

#ifndef IMAGE_WIDTH
#define IMAGE_WIDTH 10
#endif

#ifndef RESULT_FILE
#define RESULT_FILE "filename"  //Replace filename with the file containing the convolution result for the given image and kernel
#endif

#ifndef RESULT_SIZE
#define RESULT_SIZE 100
#endif

int read_img(FILE *fp, float *img);

int read_result(FILE *fp, float *result);

#endif
