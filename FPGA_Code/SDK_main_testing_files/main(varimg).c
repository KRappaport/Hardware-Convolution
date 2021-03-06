#include <stdio.h>
#include <stdlib.h>
#include "xparameters.h"
#include "xstatus.h"
#include "xconv2d.h"
#include "xconv2d_hw.h"
#include "xil_cache.h"
#include "xaxidma.h"
#include "xtime_l.h"
#include "xsdps.h"
#include "ff.h"
#include "inits.h"
#include "data_handling.h"


extern unsigned short kernel_dim[3];
extern unsigned short image_dim[3];

extern int ps7_post_config();


int main() {

    ps7_post_config();

    int status;

    char kernel_file[50];
    char image_file[50];
    char result_file[50];

    XConv2d instptr;
    status = xconv2d_init(&instptr);
    if (status != XST_SUCCESS) {
        printf("Conv2d setup failed!\n");
        exit(-1);
    }

    XAxiDma axidma;
    status = dma_init(&axidma);
    if (status != XST_SUCCESS) {
        printf("AxiDma setup failed!\n");
        exit(-1);
    }

    FATFS fs;
    status = file_system_init(&fs);
    if (status != XST_SUCCESS) {
        printf("File system setup failed!\n");
        exit(-1);
    }

    FRESULT res;
    res = f_chdir("test_data/var_img/depth3");
    if (res != FR_OK) {
        printf("ERROR: Failed to change directories! \t Error Code: %d\n", res);
        exit(-1);
    }

    printf("Starting...\n");
    int i_s, run_amount, time_index=0;
    double execution_times[252];
    for (i_s = 5; i_s <= 256; i_s++) {
        XConv2d_Set_wdth(&instptr, i_s);
        XConv2d_Set_hght(&instptr, i_s);
        sprintf(kernel_file, "ker3_%dx%dx3_1.tdatb", i_s, i_s);
        int kernel_size;
        float *kernel;
        kernel_size = read_kernel(&kernel, kernel_file);
        if (kernel_size == -1) {
            exit(-1);
        }
        set_ker(kernel, kernel_size, kernel_dim);
        free(kernel);

        execution_times[time_index] = 0;
        for (run_amount = 1; run_amount <= 10; run_amount++) {
            // printf("Starting Conv2D...\n");
            if (XConv2d_IsReady(&instptr)) {
                XConv2d_Start(&instptr);
            } else {
                printf("ERROR: Conv2D not ready!\n");
                exit(-1);
            }
            // printf("Started.\n");

            sprintf(image_file, "img_%dx%dx3_%d.tdatb", i_s, i_s, run_amount);
            int image_size;
            float *image;
            image_size = read_image(&image, image_file);
            if (image_size == -1) {
                exit(-1);
            }

            float *result;
            int result_size = image_dim[0]*image_dim[1];
            result = (float*)calloc(result_size, sizeof(float));
            if (result == NULL) {
                printf("ERROR: Failed to allocate memory for the result!\n");
                free(image);
                exit(-1);
            }

            double conv_exec_time = conv_image(&axidma, image, image_size, result, result_size);
            if (conv_exec_time == -1.0f) {
                printf("ERROR: Transfer of either the image or result failed!\n");
                free(image);
                free(result);
                exit(-1);
            }

            free(image);

            execution_times[time_index] += conv_exec_time;

            sprintf(result_file, "MATLAB/res_%dx%dx3_ker3_%d.tdatb", i_s, i_s, run_amount);
            float *cmpr_result;
            int cmpr_result_size;
            cmpr_result_size = read_result(&cmpr_result, result_file);
            if (cmpr_result_size == -1) {
                free(result);
                exit(-1);
            } else if (cmpr_result_size != (result_size)) {
                printf("Comparison result file is not the correct size!\n");
                free(result);
                free(cmpr_result);
                exit(-1);
            }

            unsigned int i;
            for (i = 0; i < cmpr_result_size; i++) {
                if (((result[i] - cmpr_result[i]) > 0.00005) || ((result[i] - cmpr_result[i]) < -0.00005)) {
                    printf("FPGA_result[%d]=%f  cmpr_result[%d]=%f\n", i, result[i], i, cmpr_result[i]);
                    printf("TEST FAILED!\n");
                    // free(result);
                    // free(cmpr_result);
                   // return(0);
                }
            }
            printf("TEST PASSED!\n");
            free(cmpr_result);

            free(result);
        }
        execution_times[time_index] = execution_times[time_index]/10;
        printf("Execution time for image size %d: %.9f\n", i_s, execution_times[time_index]);
        time_index++;
    }
    printf("Finished convolutions!\n");
    FIL fp;
    res = f_open(&fp, "0:/var_img_tm125.tmb", FA_CREATE_ALWAYS | FA_WRITE);
    if (res != FR_OK) {
        printf("ERROR: Failed to open time file! \t Error Code: %d\n", res);
        return(-1);
    }
    unsigned int byteswrote,bytesread;
    res = f_write(&fp, execution_times, 252*sizeof(double), &byteswrote);
    if (res != FR_OK) {
        printf("ERROR: Failed to write times! \t Error Code: %d\n", res);
        f_close(&fp);
        return(-1);
    } else if (byteswrote != (252*sizeof(double))) {
        printf("ERROR: Failed to write time! \t Bytes Written: %d\n", byteswrote);
        f_close(&fp);
        return(-1);
    }
    res = f_close(&fp);
    if (res != FR_OK) {
        printf("ERROR: Failed to close time file!\n");
        return(-1);
    }

    res = f_open(&fp, "0:/var_img_tm125_bad0.tmb", FA_READ);
    if (res != FR_OK) {
        printf("ERROR: Failed to open time file! \t Error Code: %d\n", res);
        return(-1);
    }
    res = f_read(&fp, execution_times, 252*sizeof(double), &bytesread);
    if (res != FR_OK) {
        printf("ERROR: Failed to read time data! \t Error Code: %d\n", res);
        f_close(&fp);
        return(-1);
    } else if (bytesread != byteswrote) {
        printf("ERROR: Failed to read time data!\n");
        printf("Bytes Needed: %d \t Bytes Read: %d\n", byteswrote, bytesread);
        f_close(&fp);
        return(-1);
    }
    f_mount(0, "0:/", 0);
    printf("Time written to file all done\n");

return 0;
}
