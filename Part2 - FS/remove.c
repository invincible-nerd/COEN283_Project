#include <stdio.h>
#include <stdlib.h>
#include "fs.h"

int main(int argc, char *argv[]) {
    char *name = argv[1];
    mount_fs();
    delete_file(name);
    sync_fs();
    return 0;
}