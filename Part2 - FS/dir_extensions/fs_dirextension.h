//
// Created by zongbaicheng on 2023/3/6.
//

#include "../fs.h"

#ifndef COEN283_PROJECT_FILESYSTEM_FS_DIREXTENSION_H
#define COEN283_PROJECT_FILESYSTEM_FS_DIREXTENSION_H

#endif //COEN283_PROJECT_FILESYSTEM_FS_DIREXTENSION_H


void show_path(); // like pwd, just show the current path
void jump_in(char name[FILENAME_LEN]); //jump to directory using name under current path
void jump_in_by_path(char *path); //jump to directory using the absolute path

int create_dir(char name[FILENAME_LEN]);
void delete_dir(char name[FILENAME_LEN]);