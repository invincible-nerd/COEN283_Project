#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "BasicStructures.c" // assume the program is in a separate header file

int main(void) {
    struct DISK disk;
    char data[1023];
    int status;

    // Write data to the disk
    memset(data, 'AAA', sizeof(data));
    status = Write_Data_to_Disk(&disk, data, sizeof(data));
    if (status == NO_BLOCK_ERROR) {
        printf("Failed to write data to disk\n");
        return 1;
    }
    printf("Wrote data to disk at location %d\n", status);

    // Read data back from the disk
    memset(data, 0, sizeof(data));
    Read_Data_from_Disk(&disk, data, sizeof(data), status);
    if (memcmp(data, disk.disk_data + status, sizeof(data)) != 0) {
        printf("Data read from disk does not match what was written\n");
        return 1;
    }
    printf("Successfully read data from disk\n");

    // Write data to the disk
    memset(data, 'AAA', sizeof(data));
    status = Write_Data_to_Disk(&disk, data, sizeof(data));
    if (status == NO_BLOCK_ERROR) {
        printf("Failed to write data to disk\n");
        return 1;
    }
    printf("Wrote data to disk at location %d\n", status);

    // Read data back from the disk
    memset(data, 0, sizeof(data));
    Read_Data_from_Disk(&disk, data, sizeof(data), status);
    if (memcmp(data, disk.disk_data + status, sizeof(data)) != 0) {
        printf("Data read from disk does not match what was written\n");
        return 1;
    }
    printf("Successfully read data from disk\n");


    // Try to write data to a full disk
    memset(data, 'B', sizeof(data));
    for (int i = 0; i < MAX_BLOCK; i++) {
        status = Write_Data_to_Disk(&disk, data, sizeof(data));
        if (status == NO_BLOCK_ERROR) {
            printf("Disk is full, cannot write more data\n");
            break;
        }
    }

    return 0;
}
