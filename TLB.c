#include "PageFaultHandler.c"



// 4.1 Define basic TLB structures:
// 4.1.1 struct TLB_ENTRY: include page_num and frame_num
// 4.1.2 struct (or just an array) TLB_TABLE (fixed size = TLB_SIZE)
typedef struct TLB_ENTRY{
    int is_empty;   //1 is empty, 0 is occupied
    int page_num;
    int frame_num;
};

typedef struct TLB{
    struct TLB_ENTRY tlb_entries[TLB_SIZE];
};

// 4.2 Implement function search_TLB(): search the TLB for the desired page number's data
void searchTLB(uint16_t virtual_address, struct TLB *tlb, struct PAGE_TABLE *page_table, struct PHYSICAL_MEMORY *physical_memory, struct DISK *disk){
    //return value frame number's range is [0,15]
    uint16_t page_number=Get_Page_Number(virtual_address);
    uint16_t page_offset=Get_Page_Offset(virtual_address);
    
    int i=0;
    for(; i<TLB_SIZE; i++){
        if(tlb->tlb_entries[i].is_empty==1){
            //reached the end but still not found
            break;
        }
        if(tlb->tlb_entries[i].page_num==page_number){
            //4.2.1 Found => skip check page table and directly get frame number
            //return tlb->tlb_entries[i]->frame_num;
            printf("Cache found in Translate Link Buffer.\n");
            printData(physical_memory,tlb->tlb_entries[i].frame_num,page_offset);
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
        tlb->tlb_entries[i].page_num=page_number;
        tlb->tlb_entries[i].is_empty=0;
        tlb->tlb_entries[i].frame_num=page_table->page_entries[page_number].frame_number;
    }else{
        //overwrite entries[0]
        tlb->tlb_entries[i].page_num=page_number;
        tlb->tlb_entries[i].is_empty=0;
        tlb->tlb_entries[i].frame_num=page_table->page_entries[page_number].frame_number;
    }
}


