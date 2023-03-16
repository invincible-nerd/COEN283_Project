#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "TLB.c"


int main(void){
    struct DISK disk;
    char data[1023];
    int status;
    //Disk initialization
    memset(data,'AAA',sizeof(data));

    //Physical memory initialization
     struct PHYSICAL_MEMORY memory;
     for (int i = 0; i < FRAME_NUM; i++){
         memory.frames[i].is_empty=1;
     }
    //struct FRAME initial_frame={0,'AAAAAAAAAAAAAAAAAA'};
    memory.frames[1].is_empty=0;
    memset(memory.frames[1].frame_data,'AAA',sizeof(memory.frames[1]));
    
    //Page Table initialization
    struct PAGE_TABLE page_table;
    for (int i = 0; i < PAGE_NUM; i++){
         page_table.page_entries[i].protection_bits=0;
         page_table.page_entries[i].dirty_bit=0;
         page_table.page_entries[i].reference_bit=0;
         page_table.page_entries[i].valid_bit=0;
    }
    //struct PAGE_TABLE_ENTRY initial_entry={0,0,0,0,1};
    //map: 0->1
    page_table.page_entries[0].frame_number=1;
    page_table.page_entries[0].valid_bit=1;
    //TLB initialization
    struct TLB tlb;
    for (int i = 0; i < TLB_SIZE; i++){
         tlb.tlb_entries[i].is_empty=1;
    }
    //our process: 
    //virtual address to access:
    //(0,5), (1,0), (1,1), (0,5), (1,0)
    //(0,5): not in TLB, in page table
    //update TLB
    searchTLB(5,&tlb,&page_table,&memory,&disk);
    printf("================================.\n");
    //(1,1): not in TLB, not in page table
    //need to bring into memory frame  
    //also update TLB
    searchTLB(1025,&tlb,&page_table,&memory,&disk);
    printf("================================.\n");
    //(2,1): not in TLB, not in page table
    //need to bring into memory frame
    //also update TLB
    searchTLB(2049,&tlb,&page_table,&memory,&disk);
    printf("================================.\n");
    //(0,5): in TLB, (in page table)
    searchTLB(5,&tlb,&page_table,&memory,&disk);
    printf("================================.\n");
    //(1,0): in TLB, (in page table)
    searchTLB(1024,&tlb,&page_table,&memory,&disk);
    


    
    return 0;
}
