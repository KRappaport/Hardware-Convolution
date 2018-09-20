// The following is a simple C code which takes a single command line argument
// of the result filename to read and will then print out the data read
// from the file.  The files should have the proper format; 2 short numbers for
// the dimensions and then the correct amount of floats corresponding to the
// specified dimensions.

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        printf("ERROR: Problem with commands.\n");
        printf("Plaese specify a result file to be read.\n");
        return(0);
    }
    FILE *fp = NULL;
    if ((fp = fopen(argv[1], "rb")) == NULL) {
        perror("Couldn't open file");
        exit(EXIT_FAILURE);
    }

    unsigned short dim[2];
    if (fread(dim, sizeof(short), 2, fp) != 2) {
        perror("Problem reading data dimensions");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    size_t data_size = dim[0]*dim[1];
    float *data = (float*)calloc(data_size, sizeof(float));
    if (data == NULL) {
        perror("Couldn't allocate memory");
        fclose(fp);
        exit(EXIT_FAILURE);
    }
    if (fread(data, sizeof(float), data_size, fp) != data_size) {
        perror("Problem reading data");
        fclose(fp);
        free(data);
        exit(EXIT_FAILURE);
    }

    fclose(fp);

    printf("Dimensions: %dx%d\n", dim[0], dim[1]);

    size_t i;
    for (i = 0; i < data_size; i++) {
        printf("%f  ", data[i]);
    }
    printf("\n");

    free(data);

    return 0;
}
