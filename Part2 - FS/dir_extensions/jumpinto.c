#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../fs.h"
#include "fs_dirextension.h"

int main(int argc, char *argv[]) {
    char *name = argv[1];
    mount_fs();
    if (strchr(name, '/') == NULL){
        jump_in(name);
    } else {
        jump_in_by_path(name);
    }
    sync_fs();
    return 0;
}