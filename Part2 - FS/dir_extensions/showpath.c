#include "../fs.h"
#include "fs_dirextension.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    mount_fs();
    printf("The current path is: ");
    show_path();
    return 0;
}