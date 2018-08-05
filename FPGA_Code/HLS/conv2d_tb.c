#include <stdio.h>
#include "conv2d.h"
#include "test_bench.h"

int main() {
    int i,k;
    float out[RESULT_SIZE];
    float ker[DEPTH][KERNEL_DIM_SQR];
    float img[IMAGE_HEIGHT*IMAGE_WIDTH*DEPTH];
    FILE *fp_img=NULL;
    FILE *fp_ker=NULL;
    FILE *fp_res=NULL;

    if ((fp_ker = fopen(KERNEL_FILE, "rb")) == NULL) {
        perror("Couldn't open file!");
        return(-1);
    }
    unsigned short dim[3];
    if (fread(dim, sizeof(short), 3, fp_ker) != 3) {
        perror("Problem reading kernel dimensions!");
        fclose(fp_ker);
        return(-1);
    }
    size_t data_size = dim[0]*dim[1];
    if (fread(ker[0], sizeof(float), data_size, fp_ker) != data_size) {
        perror("Problem reading kernel!");
        fclose(fp_ker);
        return(-1);
    }
    fclose(fp_ker);
    for (i = 0; i < KERNEL_DIM_SQR; i++) {
        ker[1][i] = ker[0][i];
        ker[2][i] = ker[0][i];
        // printf("ker[0][%d] = %f\n", i, ker[0][i]);
    }


    if ((fp_img = fopen(IMAGE_FILE, "rb")) == NULL) {
        perror("Couldn't open file!");
        return(-1);
    }
    if (read_img(fp_img, img)==-1) {
        fclose(fp_img);
        return(-1);
    }
    fclose(fp_img);
    // printf("img[0]=%f\n", img[0]);

    conv2d(img, ker, IMAGE_WIDTH, IMAGE_HEIGHT, out);
    // free(img);

    if ((fp_res = fopen(RESULT_FILE, "rb")) == NULL) {
        perror("Couldn't open file!");
        return(-1);
    }
    if (read_result(fp_res, img) == -1) {
        fclose(fp_res);
        return(-1);
    }
    fclose(fp_res);

    for (i = 0; i < RESULT_SIZE; i++) {
        if (((out[i] - img[i]) > 0.005) || ((out[i] - img[i]) < -0.005)) {
            printf("out[%d]=%f  result[%d]=%f\n", i, out[i], i, img[i]);
            printf("TEST FAILED!\n");
            // free(result);
            return(0);
        }
    }
    printf("TEST PASSED!\n");
    // free(result);

    return 0;
}
