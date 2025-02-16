#include "common/common.h"
#include <stdio.h>

void commonTest() {
    printf("Hello from COMMON.\n");

    #ifdef DEBUG_MODE
        printf("COMMON DEBUG MODE.\n");
    #endif
}