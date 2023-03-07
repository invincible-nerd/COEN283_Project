//
// Created by zongbaicheng on 2023/2/27.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fs.h"

/*
struct superblock {
    int num_inodes;
    int num_blocks;
    int size_blocks;
    int current_directory; // the id of the directory we are currently at, -1 (initialized number) means no directory is under
};

struct inode {
    int size;
    int first_block;
    char name[FILENAME_LEN];
    int isFile; // 1: is a regular file, 0: is a directory
    int parent; // the id of the directory which contains the current file, -1 means no parent
};

struct disk_block {
    int next_block_num;
    char data[BLOCKSIZE];
};
*/

struct superblock sb;
struct inode *inodes;
struct disk_block *dbs;

int find_empty_inode() {
    for (int i = 0; i < sb.num_inodes; ++i) {
        if (inodes[i].first_block == -1) {
            return i;
        }
    }
    return -1;
} // find empty inode

int find_empty_block() {
    for (int i = 0; i < sb.num_blocks; ++i) {
        if (dbs[i].next_block_num == -1) {
            return i;
        }
    }
    return -1;
} // find empty block


int get_block_num(int file, int offset) {
    int togo = offset;
    int bn = inodes[file].first_block;

    while (togo > 0){
        bn = dbs[bn].next_block_num;
        togo -= 1;
    }
    return bn;
}

// initialize new filesystem
void create_fs(){
    sb.num_inodes = 10;
    sb.num_blocks = 100;
    sb.size_blocks = sizeof (struct disk_block);
    sb.current_directory = -1;

    inodes = malloc(sizeof(struct inode) * sb.num_inodes);
    for (int i = 0; i < sb.num_inodes; ++i) {
        strcpy(inodes[i].name, "");
        inodes[i].size = -1;
        inodes[i].first_block = -1;
        inodes[i].isFile = -1;
        inodes[i].parent = -1;
    }

    dbs = malloc(sizeof (struct disk_block) * sb.num_blocks);
    for (int i = 0; i < sb.num_blocks; ++i) {
        dbs[i].next_block_num = -1;
    }

}

// load a file system
void mount_fs(){
    FILE *file;
    file = fopen("fs_data", "r");

    // superblock
    fread(&sb, sizeof (struct superblock), 1, file);

    // inodes
    inodes = malloc(sizeof (struct inode) * sb.num_inodes);
    dbs = malloc(sizeof (struct disk_block) * sb.num_blocks);
    fread(inodes, sizeof (struct inode), sb.num_inodes, file);
    fread(dbs, sizeof (struct disk_block), sb.num_blocks, file);
    fclose(file);
}

// write the file system
void sync_fs(){
    FILE *file;
    file = fopen("fs_data", "w+");

    // superblock
    fwrite(&sb, sizeof (struct superblock), 1, file);

    // inodes
    for (int i = 0; i < sb.num_inodes; ++i) {
        fwrite(&(inodes[i]), sizeof (struct inode), 1, file);
    }

    for (int i = 0; i < sb.num_blocks; ++i) {
        fwrite(&(dbs[i]), sizeof (struct disk_block), 1, file);
    }

    fclose(file);
}

void print_fs() {
    printf("Superblock info\n");
    printf("\tnumber of inodes %d\n", sb.num_inodes);
    printf("\tnumber of blocks %d\n", sb.num_blocks);
    printf("\tsize of each block %d\n", sb.size_blocks);
    printf("\tcurrent directory index %d\n", sb.current_directory);

    printf("inodes\n");
    for (int i = 0; i < sb.num_inodes; ++i) {
        if (inodes[i].isFile == 1){
            printf("\tsize: %d block: %d name: %s type: %s\n", inodes[i].size, inodes[i].first_block,inodes[i].name, "regular");
        } else if (inodes[i].isFile == 0){
            printf("\tsize: %d block: %d name: %s type: %s\n", inodes[i].size, inodes[i].first_block,inodes[i].name, "directory");
        } else {
            printf("\tsize: %d block: %d name: %s type: %s\n", inodes[i].size, inodes[i].first_block,inodes[i].name, "empty");
        }
        
    }

    for (int i = 0; i < sb.num_blocks; ++i) {
        printf("\tblock idx: %d next block idx: %d\n", i, dbs[i].next_block_num);
    }

}


int allocate_file(char name[FILENAME_LEN]){
    for (int i = 0; i < sb.num_inodes; ++i) {
        if (strcmp(inodes[i].name, name) == 0){
            printf("ERROR: file name: '%s' already in the current path please change another one.\n", name);
            return ERROR_SAME_FILENAME;
        }
    }
    // find an empty inode
    int in = find_empty_inode();
    // find / claim a disk block
    int block = find_empty_block();
    // claim them
    inodes[in].first_block = block;
    inodes[in].size = 0;
    inodes[in].isFile = 1;
    inodes[in].parent = sb.current_directory;
    dbs[block].next_block_num = -2;
    strcpy(inodes[in].name, name);
    return in;
}

// add / delete blocks
void set_filesize(int filenum, int size) {
    // how many blocks should we have (round up)
    int tmp = size + BLOCKSIZE - 1;
    int num = tmp / BLOCKSIZE;
//    printf("num: %d, size: %d\n", num, size);
    int bn = inodes[filenum].first_block;
    num -= 1;
    while (num > 0){
        // check next block number
        int next_num = dbs[bn].next_block_num;
        if (next_num == -2) {
            int empty = find_empty_block();
            dbs[bn].next_block_num = empty;
            dbs[empty].next_block_num = -2;
        }
        bn = dbs[bn].next_block_num;
        num -= 1;
    }

    // shorten as needed
//    shorten_file(bn);
    dbs[bn].next_block_num = -2;

    // set inode size
    inodes[filenum].size = size;

} // set_filesize

int get_file_number_by_name(char name[FILENAME_LEN]){
    for (int i = 0; i < sb.num_inodes; ++i) {
        if (strcmp(name, inodes[i].name) == 0){
            return i;
        }
    }
    return ERROR_FILE_NOT_FOUND;
}

int allocate_file_with_size(char name[FILENAME_LEN], int size){
    allocate_file(name);
    int filenum = get_file_number_by_name(name);
    set_filesize(filenum, size);
    return filenum;
}

int write_data(char name[FILENAME_LEN], char *data, int size) {
    int filenum = get_file_number_by_name(name);
    if (filenum == ERROR_FILE_NOT_FOUND){
        printf("File: %s is not found, please try again.\n", name);
        return ERROR_FILE_NOT_FOUND;
    }
    if (inodes[filenum].size < size) {
        printf("Write data size is larger than file size, please rewrite.");
        return ERROR_FILE_OVERSIZE;
    }
    // calculate how many blocks needed
    int left_size = size;
    int cb_num = inodes[filenum].first_block;
    int dp = 0;
    while (left_size > 0 && cb_num >= 0) {
        struct disk_block *cb = &dbs[cb_num];
        // copy data
        int copysize = strlen(data+dp)+1 < BLOCKSIZE ? strlen(data+dp)+1 : BLOCKSIZE;
        strncpy(cb->data, data+dp, copysize);
//        printf("data: %d, %d, %d\n", strlen(data+dp)+1, BLOCKSIZE, copysize);
//        printf("data: %s\n", cb->data);
        dp += BLOCKSIZE;
        cb_num = cb->next_block_num;
        left_size -= BLOCKSIZE;
    }
    printf("Successfully write data in file: %s\n", name);
    return VALID_STATUS;
}

void list_files(){
    for (int i = 0; i < sb.num_inodes; ++i) {
        if (inodes[i].first_block != -1 && inodes[i].parent == sb.current_directory) {
            if (inodes[i].isFile == 1){
                printf("File Name: %s, File Size: %d, File Type: Regular File\n", inodes[i].name, inodes[i].size);
            } else if (inodes[i].isFile == 0){
                printf("File Name: %s, File Size: %d, File Type: Directory\n", inodes[i].name, inodes[i].size);
            }
        }
    }
}

void delete_file(char name[FILENAME_LEN]) {
    int flag = 0;
    for (int i = 0; i < sb.num_inodes; ++i) {
        if (strcmp(inodes[i].name, name) == 0) {
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

// Be careful!! This function will clear all data from the disk
void format_disk(){
    char str[10];
    printf("Are you sure to clear the disk and all saved data will be removed (Y/N) ? ");
    scanf("%s", str);
    if (strcmp(str, "Y") == 0) {
        // format inodes
        for (int i = 0; i < sb.num_inodes; ++i) {
            strcpy(inodes[i].name, "");
            inodes[i].first_block = -1;
            inodes[i].size = -1;
        }
        // format blocks
        for (int i = 0; i < sb.num_blocks; ++i) {
            dbs[i].next_block_num = -1;
            memset(dbs[i].data, 0, BLOCKSIZE);
        }
        printf("Successfully format the disk.\n");
        return;
    }
    printf("Cancelled!\n");
}

void show_file(char name[FILENAME_LEN]){
    for (int i = 0; i < sb.num_inodes; ++i) {
        if (strcmp(inodes[i].name, name) == 0) {
            printf("FILE NAME: %s\n", name);
            if (inodes[i].isFile == 1){
                printf("FILE TYPE: General File\n");
                printf("CONTENT: ");
                int cb_num = inodes[i].first_block;
                while (cb_num >= 0) {
                    printf("%s\n", dbs[cb_num].data);
                    cb_num = dbs[cb_num].next_block_num;
                }
                break;
            } else if (inodes[i].isFile == 0){
                printf("FILE TYPE: Directory\n");
                printf("FILES IN THIS DIRECTORY: ");
                int curr = i;
                for (int j = 0; j < sb.num_inodes; j++)
                {
                    if (inodes[j].parent == curr) {
                        printf("%s ", inodes[j].name);
                    }
                }
            }
        }
    }
    printf("\n");
}