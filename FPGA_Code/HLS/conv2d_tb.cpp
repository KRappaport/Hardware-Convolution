#include <stdio.h>
#include "conv2d.h"
#include "test_bench.h"

int main() {
    int i,k;
    float res[RESULT_SIZE];
    float ker[DEPTH][KERNEL_DIM_SQR];
    float img[IMAGE_HEIGHT*IMAGE_WIDTH*DEPTH];
    AXIS_STRUCT image_data, result_data;
    AXIS_PORT image, result;
    FILE *fp_img=NULL;
    FILE *fp_ker=NULL;
    FILE *fp_res=NULL;

    image_data.last = 0;
    image_data.keep = 0xF;
    image_data.strb = 0xF;

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

    for (i = 0; i < ((IMAGE_HEIGHT*IMAGE_WIDTH*DEPTH)-1); i++) {
        image_data.data = img[i];
        image.write(image_data);
    }
    image_data.data = img[i];
    image_data.last = 1;
    image.write(image_data);

    // printf("img[0]=%f\n", img[0]);

    conv2d(image, ker, IMAGE_WIDTH, IMAGE_HEIGHT, result);
    // free(img);

    if ((fp_res = fopen(RESULT_FILE, "rb")) == NULL) {
        perror("Couldn't open file!");
        return(-1);
    }
    if (read_result(fp_res, res) == -1) {
        fclose(fp_res);
        return(-1);
    }
    fclose(fp_res);

    for (i = 0; i < RESULT_SIZE; i++) {
        result_data = result.read();
        if (((result_data.data - res[i]) > 0.005) || ((result_data.data - res[i]) < -0.005)) {
            printf("SIM_result[%d]=%f  MATLAB_result[%d]=%f\n", i, result_data.data, i, res[i]);
            printf("TEST FAILED!\n");
            // free(result);
            return(0);
        }
    }
    printf("TEST PASSED!\n");
    // free(result);

    return 0;
}
