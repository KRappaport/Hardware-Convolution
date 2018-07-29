#include <stdio.h>
#include "conv2d.h"

int main() {
    int i,k;
    float out[25];
    float ker[1][9] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
    float img[25] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0,
                16.0, 17.0, 18.0, 19.0, 20.0, 21.0, 22.0, 23.0, 24.0, 25.0};


    conv2d(img, ker, 5, 5, out);
    for (k = 0; k < 5; k++) {
    	for(i = 0; i<5; i++){
    	   	printf("result[%d][%d] = %f\t", k, i, out[(k*5)+i]);
    	}
    	printf("\n");
	}
    return 0;
}
