#include "xparameters.h"
#include "xsdps.h"
#include "ff.h"
#include "inits.h"
#include "xstatus.h"


int file_system_init(FATFS *fs) {
    FRESULT res;
    TCHAR *path = "0:/";

    res = f_mount(fs, path, 0);
    if (res != FR_OK) {
        return(XST_FAILURE);
    }

    return(XST_SUCCESS);
}
