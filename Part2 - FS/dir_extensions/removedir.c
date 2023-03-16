#include <stdio.h>
#include <stdlib.h>
#include "../fs.h"
#include "fs_dirextension.h"

int main(int argc, char *argv[]) {
    char *name = argv[1];
    mount_fs();
    delete_dir(name);
    sync_fs();
    return 0;
}