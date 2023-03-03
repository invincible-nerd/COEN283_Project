#include "PageFaultHandler.c"


// 4.0 Define a small constant TLB_SIZE (e.g. 64) => number of entries in TLB
#define TLB_SIZE 64;
// 4.1 Define basic TLB structures:
// 4.1.1 struct TLB_ENTRY: include page_num and frame_num
// 4.1.2 struct (or just an array) TLB_TABLE (fixed size = TLB_SIZE)
typedef struct TLB_ENTRY{
    int page_num;
    int frame_number;
}

typedef struct TLB{
    struct TLB_ENTRY tlb_entries[TLB_SIZE];
}

// 4.2 Implement function search_TLB(): search the TLB for the desired page number's data
void searchTLB(uint16_t virtual_address, struct TLB *tlb, struct PAGE_TABLE *page_table, struct PHYSICAL_MEMORY *physical_memory, struct DISK *disk){
    //return value frame number's range is [0,15]
    uint16_t page_number=Get_Page_Number(virtual_address);
    uint16_t page_offset=Get_Page_Offset(virtual_address);
    struct TLB_ENTRY entries=tlb->tlb_entries;
    int i=0;
    for(; i<TLB_SIZE; i++){
        if(entries[i]==NULL){
            //reached the end but still not found
            break;
        }
        if(entries[i].page_num==page_number){
            //4.2.1 Found => skip check page table and directly get frame number
            //return tlb->tlb_entries[i]->frame_num;
            printData(physical_memory,entries[i].frame_number,page_offset);
            return;
        }
    }
    // 4.2.2 Not Found:
//  4.2.2.1 check/calculate frame number using function in part 2 and 3
//  4.2.2.2 update TLB_TABLE by recent referenced page number (Implement function update_TLB())
//  4.2.2.3 when there's not enough TLB_TABLE entries, replace it by proper algorithm
    
    Page_Table_Handler(virtual_address,page_table,physical_memory,disk);
    if(i<TLB_SIZE){
        //add new entry at i
        entries[i].page_num=page_number;
        entries[i].frame_num=page_table->page_entries[page_number].frame_number;
    }else{
        //overwrite entries[0]
        entries[0].page_num=page_number;
        entries[0].frame_num=page_table->page_entries[page_number].frame_number;
    }
}


