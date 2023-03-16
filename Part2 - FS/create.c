#include <stdio.h>
#include <stdlib.h>
#include "fs.h"

int main(int argc, char *argv[]) {
    char *name = argv[1];
    int size = atoi(argv[2]);
    mount_fs();
    allocate_file_with_size(name, size);
    sync_fs();
    return 0;
}