#include "PX_2dxMakechain.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("parameter error.\n");
        return 1;
    }
    if (PX_2dx_Make(argv[1], argv[2])) {
        printf("Compile done.\n");
        return 0;
    } else {
        printf("Compile error.\n");
        return 1;
    }
}
