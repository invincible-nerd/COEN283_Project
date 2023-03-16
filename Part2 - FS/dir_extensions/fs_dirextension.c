//
// Created by zongbaicheng on 2023/3/6.
//

#include <stdio.h>
#include <string.h>
#include "../fs.h"
#include "fs_dirextension.h"

extern struct superblock sb;
extern struct inode *inodes;
extern struct disk_block *dbs;

void show_path(){
    int curr = sb.current_directory;
    printf("/10086_file_system");
    char path[100];
    memset(path, 0, sizeof(path));
    while (curr != -1){
        char path_single[100];
        memset(path_single, 0, sizeof(path_single));
        strcat(path_single, "/");
        strcat(path_single, inodes[curr].name);
        strcat(path_single, path);
        strcpy(path, path_single);
        curr = inodes[curr].parent;
    }
    printf("%s\n", path); 
}

void jump_in(char name[FILENAME_LEN]){
    if (strcmp(name, "..") == 0) {
        if (sb.current_directory == -1){
            printf("The given path is not valid, please try again.\n");
            return;
        }
        sb.current_directory = inodes[sb.current_directory].parent;
        printf("Successfully jump back to the last level path.\n");
        return;
    }

    int flag = 0;
    for (int i = 0; i < sb.num_inodes; ++i) {
        if (inodes[i].parent == sb.current_directory){
            flag = 1;
            if (strcmp(inodes[i].name, name) == 0){
                if (inodes[i].isFile == 1) {
                    printf("Error! you cannot jump into a genera file, please provide a directory name.\n");
                    return;
                }
                sb.current_directory = i;
            }
        }
    }
    if (flag == 0){
        printf("Directory not found, please enter a correct directory name.\n");
        return;
    }
    printf("Successfully jump into the directory %s\n", name);
}

void jump_in_by_path(char *path){
    if (strcmp(path, "/10086_file_system") == 0) {
        sb.current_directory = -1;
        printf("Successfully jump into the path %s\n", path);
        return;
    }
    
    char* token = strtok(path, "/");
    int curr = -1;
    int flag = 0;
    while (token != NULL) {
        flag = 0; // 0 means not found, 1 means found
        for (int i = 0; i < sb.num_inodes; ++i) {
            if (inodes[i].parent == curr && strcmp(inodes[i].name, token) == 0){
                flag = 1;
                curr = i;
            }
        }
        if (flag == 0){
            printf("The given path is not valid, please try again.\n");
            return;
        }
        token = strtok(NULL, "/");
    }

    sb.current_directory = curr;
    printf("Successfully jump into the path %s\n", path);
}

int allocate_dir(char name[FILENAME_LEN]){
    for (int i = 0; i < sb.num_inodes; ++i) {
        if (strcmp(inodes[i].name, name) == 0){
            printf("ERROR: file name: '%s' already in the current path please change another one.\n", name);
            return ERROR_SAME_FILENAME;
        }
    }
    // find an empty inode
    int in = find_empty_inode();
//    // find / claim a disk block
//    int block = find_empty_block();
    // claim them
    inodes[in].first_block = -2; // for directory no block needed to allocate
    inodes[in].size = 0;
    inodes[in].isFile = 0;
    inodes[in].parent = sb.current_directory;
//    dbs[block].next_block_num = -1;
    strcpy(inodes[in].name, name);
    return in;
}

int create_dir(char name[FILENAME_LEN]){
    allocate_dir(name);
    int dirnum = get_file_number_by_name(name);
    set_filesize(dirnum, 1000);
    printf("Successfully create directory called %s\n", name);
    return dirnum;
}

void delete_file_under(char name[FILENAME_LEN], int dirnum){
    int flag = 0;
    for (int i = 0; i < sb.num_inodes; ++i) {
        if (inodes[i].parent == dirnum && strcmp(inodes[i].name, name) == 0) {
            // set flag
            flag = 1;
            // delete file with filenum = i
            // clear block
            struct inode *in = &inodes[i];
            int cb_num = in->first_block;
            while (cb_num > 0){
                struct disk_block *cb = &dbs[cb_num];
                // clear block data
                memset(cb->data, 0, BLOCKSIZE);
                cb_num = cb->next_block_num;
                // clear next block index
                cb->next_block_num = -1;
            }
            if (cb_num == -2){
                dbs[cb_num].next_block_num = -1;
            }
            // clear inode
            in->first_block = -1;
            // clear inode name
            strcpy(in->name, "");
            // set size to -1
            in->size = -1;
            in->isFile = -1;
        }
    }
    if (!flag) {
        printf("No file called: %s, please try another one.\n", name);
    } else {
        printf("Successfully deleted file: %s\n", name);
    }
}

int clear_dir(int dirnum){
    int flag = 0;
    for (int i = 0; i < sb.num_inodes; ++i) {
        if (inodes[i].parent == dirnum) {
            flag = 1;
            if (inodes[i].isFile == 1){
                // delete file
                delete_file_under(inodes[i].name, dirnum);
            } else if (inodes[i].isFile == 0) {
                // delete dir
                //1. clear dir
                clear_dir(i);
                //2. delete dir itself
                delete_file_under(inodes[i].name, dirnum);
            }
        }
    }
    if (flag == 0){
        return ERROR_FILE_NOT_FOUND;
    }
    return VALID_STATUS;
}

void delete_dir(char name[FILENAME_LEN]){
    int dirnum = get_file_number_by_name(name);
    if (inodes[dirnum].isFile == 1){
        printf("ERROR, please enter a valid directory name!\n");
        return;
    }
    int STATUS = clear_dir(dirnum);
    if (STATUS == VALID_STATUS){
        delete_file_under(name, sb.current_directory);
    } else{
        printf("DIR NOT FOUND, please enter a existing directory name!\n");
    }
}