#include <stdio.h>
#include <stdlib.h>
#include "fs.h"

int main(int argc, char *argv[]) {
    char *name = argv[1];
    char *data = argv[2];
    mount_fs();
    write_data(name, data, sizeof(data));
    sync_fs();
    return 0;
}