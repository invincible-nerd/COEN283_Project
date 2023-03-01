#include "constants.h"

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

int seek_free_block(int block_number, struct DISK *disk);
int Write_Data_to_Disk(struct DISK *disk, char *data, int size);
void Read_Data_from_Disk(struct DISK *disk, char *data, int size, int start_location);
uint16_t Get_Page_Number(uint16_t virtual_address);
uint16_t Get_Page_Offset(uint16_t virtual_address);
void print_binary16(uint16_t num);

