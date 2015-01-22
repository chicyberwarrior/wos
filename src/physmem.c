#include "console.h"
#include "string.h"
#include "physmem.h"

// Total memory in bytes
unsigned int pmm_total_memory_size = 0; 

// Total memory of usable regions in bytes
unsigned int pmm_usable_memory_size = 0;

// Points to the beginning of the memory map (bits to blocks)
unsigned int * pmm_memory_map = 0;

// Memory map in blocks. I.e., if total memory is 3865 bytes,
// we need 36864 / 4096 = 9 blocks to represent it. Since we need 
// one bit per block, we need to see how bytes that is: 9 / 8 = 1 + 1/9 => 2
unsigned int pmm_memory_map_length_in_blocks = 0;

// Total number of blocks
int pmm_total_blocks = 0;

// Number of blocks left for allocation
int pmm_used_blocks = 0;

int pmm_set_block(int bit, int val) {
    if(val) {
        pmm_memory_map[bit / 32] |= (1 << (bit % 32));
    } else {
        pmm_memory_map[bit / 32] &= ~ (1 << (bit % 32));
    }

    return val;
}

int pmm_get_block(int bit) {
    return (pmm_memory_map[bit / 32] &  (1 << (bit % 32))) != 0;
}

void pmm_initialize(unsigned int where, int size) {
    pmm_memory_map = (unsigned int *) where;
    pmm_total_memory_size = size;
    pmm_total_blocks = pmm_total_memory_size / PMM_BLOCK_SIZE;
    pmm_used_blocks = pmm_total_blocks; // We start with all blocks reserved. 

    pmm_memory_map_length_in_blocks = pmm_total_memory_size / PMM_BLOCK_SIZE / 8;
       
    // Mark all blocks to be reserved
    memset((unsigned int *)where, 0xFFFFFFFF, pmm_total_memory_size / PMM_BLOCK_SIZE / 8);
}

void pmm_set_memory_size(unsigned int size) {
    pmm_total_memory_size = size;
}

unsigned int pmm_get_memory_start() {
    return (unsigned int) pmm_memory_map;
}

unsigned int pmm_get_memory_size() {
    return pmm_total_memory_size;
}

int pmm_location_to_block(unsigned int loc) {
    return loc / PMM_BLOCK_SIZE;
}

/* Adds a region of usable memory */
void pmm_add_memory_region(unsigned int start, unsigned int end) {

    //log("Adding memory region %u - %u\n", start, end);
    // Starting block
    int bit = start / PMM_BLOCK_SIZE;
    
    // Number of blocks in memory region
    int num = (end - start) / PMM_BLOCK_SIZE;

    if((end - start) % PMM_BLOCK_SIZE != 0) {
        num++;
    }

    // Mark each block as available
    for(int i = 0; i < num; i++) {
        pmm_set_block(bit++, 0);
        pmm_used_blocks--;
    }
}

int pmm_next_free() {
    for(int i = 0; i < pmm_total_blocks; i++) {
        if(pmm_get_block(i) == 0) {
            return i;
        }
    }

    return -1;
}

int pmm_reserve_block(int i) {
    int ret = pmm_set_block(i, 1);
    pmm_used_blocks++;

    return ret;
}

int pmm_reserve_next_free() {
    int free = pmm_next_free();

    if(free == -1) {
        panic("Failed to reserve more memory blocks.");
    }

    pmm_reserve_block(free);

    return free;
}

int pmm_free_block(int i) {
    pmm_used_blocks--;
    return pmm_set_block(i, 0);
}

int pmm_block_status(int block) {
    return pmm_get_block(block);    
}

void pmm_print_summary() {
    printk("PMM internal summary:\n");
    printk("    pmm_mem_size: %d\n    pmm_total_blocks: %d\n    pmm_used_blocks: %d\n", 
        pmm_total_memory_size, pmm_total_blocks, pmm_used_blocks);
}

void pmm_dump_mm() {
    int w = 60;
    int r = pmm_total_blocks / 60;

    for(int i = 0; i < r; i++) {
        log("%d | ", i);
        for(int j = 0; j < w; j++) {
            log(pmm_get_block(i * w + j)?"1":"0");
        }
        log("\n");
    }
}

