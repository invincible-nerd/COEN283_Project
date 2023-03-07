#include <stdio.h>
#include <stdlib.h>
#include "fs.h"

int main(int argc, char *argv[]) {
    char *name = argv[1];
    mount_fs();
    show_file(name);
    return 0;
}