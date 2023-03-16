// Basic constants include
#define PAGE_SIZE           1024        // 2^10
#define PAGE_NUM            64          // 2^6
#define FRAME_SIZE          1024        // 2^10
#define FRAME_NUM           16          // 2^4
#define BLOCK_SIZE          1024        // 2^10
#define MAX_BLOCK           128         // 2^7

// 4.0 Define a small constant TLB_SIZE (e.g. 64) => number of entries in TLB
#define TLB_SIZE 64

// Error constants
#define NO_FREE_BLOCK      -1
#define NO_BLOCK_ERROR     -1