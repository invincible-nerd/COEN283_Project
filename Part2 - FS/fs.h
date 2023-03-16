//
// Created by zongbaicheng on 2023/2/27.
//

#ifndef COEN283_PROJECT_FILESYSTEM_FS_H
#define COEN283_PROJECT_FILESYSTEM_FS_H

// meta info about file system
// number of inodes
// number of disk blocks
// size of the disk blocks

#define BLOCKSIZE 1024
#define FILENAME_LEN 10  // filename len (including terminate sign)
#define ERROR_SAME_FILENAME -1
#define ERROR_FILE_NOT_FOUND -1
#define ERROR_FILE_OVERSIZE -1
#define VALID_STATUS 0

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

int get_file_number_by_name(char name[FILENAME_LEN]);
int find_empty_inode();
int find_empty_block();

void create_fs(); // initialize new filesystem
void mount_fs();  // load a file system
void sync_fs();   // write the file system

// return filenumber
int allocate_file(char name[FILENAME_LEN]);
int allocate_file_with_size(char name[FILENAME_LEN], int size);
void delete_file(char name[FILENAME_LEN]);
void set_filesize(int filenum, int size);
int write_data(char name[FILENAME_LEN], char *data, int size); // write data to file
void format_disk(); // clear saved data from the disk
void print_fs();   // print out info about the filesystem
void list_files(); // list all file name and size in current fs
void show_file(char name[FILENAME_LEN]); // show the content of a file

#endif //COEN283_PROJECT_FILESYSTEM_FS_H
