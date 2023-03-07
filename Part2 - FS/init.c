#include <stdio.h>
#include "fs.h"

int main(){
    create_fs();
    sync_fs();
    printf("Already initialize our file system, feel free to have a try.\n");
    return 0;
}