#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "constants.h"
<<<<<<< HEAD
#include <stdint.h>
=======
#include "BasicStructures.h"
>>>>>>> 43d8402fc6715439536562f34597efa47f1422be

/*
// 1.1 Define basic virtual memory structure
typedef struct PAGE_TABLE_ENTRY {
    int protection_bits;
    int dirty_bit;
    int reference_bit;
    int valid_bit;
    int frame_number;
};

typedef struct PAGE_TABLE {
    struct PAGE_TABLE_ENTRY page_entries[PAGE_NUM];
};

// 1.2 Define basic physical memory
typedef struct FRAME {
    int is_empty; // 1 -> empty, 0 -> occupied
    char frame_data[FRAME_SIZE]; // FRAME_SIZE should be as same as BLOCK_SIZE in disk
};

typedef struct PHYSICAL_MEMORY {
    struct FRAME frames[FRAME_NUM];
};


// 1.3 Implement DISK
// 1.3.1 Define basic secondary memory structure
typedef struct DISK {
    char disk_data[MAX_BLOCK*BLOCK_SIZE];
};
*/

// 1.3.2.0 complete function seek_free_block to find free block series
int seek_free_block(int block_number, struct DISK *disk){
    int start = -1;
    int count = 0;
    for (int i = 0; i < MAX_BLOCK; ++i) {
        if (memcmp(&disk->disk_data[i*BLOCK_SIZE], (int[BLOCK_SIZE]){0}, BLOCK_SIZE) != 0){
            count = 0;
            continue;
        }else {
            if (count == 0) {
                start = i;
            }
            count += 1;
        }
        if (count == block_number){
            return start;
        }
    }
    return NO_FREE_BLOCK;
}

// 1.3.2.1 Implement basic write function
int Write_Data_to_Disk(struct DISK *disk, char *data, int size){
    int free_location = seek_free_block(ceil(size*1.0/BLOCK_SIZE), disk);
    if (free_location == NO_FREE_BLOCK){
        printf("Error: No free space in disk.... Try to delete something\n");
        return NO_BLOCK_ERROR;
    }
    for (int i = 0; i < size; ++i) {
        disk->disk_data[free_location+i] = data[i];
    }
    return free_location;
}

// 1.3.2.2 Implement basic read function
void Read_Data_from_Disk(struct DISK *disk, char *data, int size, int start_location){
    for (int i = 0; i < size; ++i) {
        data[i] = disk->disk_data[start_location+i];
    }
}

// 1.4 Implement basic helper functions
// virtual_address is binary 16-bits integer
// the first 6 bits refer page number, the second 10 bits refer frame offset
uint16_t Get_Page_Number(uint16_t virtual_address){
    uint16_t mask = 0b1111111111;
    uint16_t first_6_bits = (virtual_address >> 10) & mask;
    return first_6_bits;
}

uint16_t Get_Page_Offset(uint16_t virtual_address){
    uint16_t mask = 0b1111111111;
    uint16_t last_10_bits = virtual_address & mask;
    return last_10_bits;
}

// helper function to print uint16_t binary number
void print_binary16(uint16_t num) {
    for(int i = 15; i >= 0; i--) {
        printf("%u", (num >> i) & 1);
    }
    printf("\n");
}
