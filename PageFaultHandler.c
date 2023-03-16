#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "constants.h"
#include "BasicStructures.c"

// 2.1 Implement function Page_Table_Handler()
int get_free_frame(struct PAGE_TABLE* page_table, struct PHYSICAL_MEMORY* physical_memory) {
    // Look for free frame
    for (int i = 0; i < FRAME_NUM; i++) {
        if (physical_memory->frames[i].is_empty == 1) {
            return i;
        }
    }

    // Use LRU algorithm to find replaced page
    int lru_queue[FRAME_NUM];
    int lru_index = 0;
    int free_frame = lru_queue[lru_index];
    lru_index = (lru_index + 1) % FRAME_NUM;
    return free_frame;

    // Set valid bit of replaced page to 0
    for (int i = 0; i < PAGE_NUM; i++) {
        if (page_table->page_entries[i].frame_number == free_frame) {
            page_table->page_entries[i].valid_bit = 0;
            break;
        }
    }

    return free_frame;
}

void load_page(struct DISK* disk, struct PHYSICAL_MEMORY* physical_memory, uint16_t page_number, int frame_number) {
    char data[FRAME_SIZE];
    Read_Data_from_Disk(disk, data, FRAME_SIZE, page_number * FRAME_SIZE);
}

void update_table(struct PAGE_TABLE* page_table, uint16_t page_number, int frame_number) {
    page_table->page_entries[page_number].valid_bit = 1;
    page_table->page_entries[page_number].frame_number = frame_number;
}

void Page_Table_Handler(uint16_t virtual_address, struct PAGE_TABLE *page_table, struct PHYSICAL_MEMORY *physical_memory, struct DISK *disk) {

    // Get the page number and page offset
    uint16_t page_number = Get_Page_Number(virtual_address);
    uint16_t page_offset = Get_Page_Offset(virtual_address);

    // Check if the page is valid, if not, handle the page fault
    struct PAGE_TABLE_ENTRY page_entry = page_table->page_entries[page_number];
    if (page_entry.valid_bit == 1) {
        printf("No Page Fault,:)\n");
    }
    else if (page_entry.valid_bit == 0) {
        printf("Page fault occurs!\n");
        // Get a free frame or select a page to replace using the FIFO algorithm
        int free_frame_number = get_free_frame(page_table, physical_memory);

        // Load the page from the disk to physical memory
        load_page(disk,physical_memory, page_number, free_frame_number);

        // Update the page table entry
        update_table(page_table, page_number, free_frame_number);

        printf("Page fault has been handled.\n");
    }

    // Access the memory in the physical memory
    char data = physical_memory->frames[page_entry.frame_number].frame_data[page_offset];
    printf("Data accessed in physical memory\n");
}

void printData(struct PHYSICAL_MEMORY *physical_memory,int frame_number, int offset ){
        // Access the memory in the physical memory
    char data = physical_memory->frames[frame_number].frame_data[offset];
    printf("Data accessed in physical memory\n");
}