#include <console.h>
#include "experiment.h"
#include <physmem.h>

void experiment() {
    unsigned int mem_start = pmm_get_memory_start();
    unsigned int mem_end = mem_start + pmm_get_memory_size();

    printk("EXPERIMENT #1\n");
    printk("Testing phys memory\n");
    printk("Memory should start at: 0x%x\n", mem_start);
    printk("And it should end at: 0x%x \n", mem_start + mem_end);

    printk("Running tests.\n"); 
   
    test_map_protected();
    test_allocate_single(); 
    test_allocate_all();
}

void test_allocate_single() {
    printk("*******************************\n");
    printk("TEST: allocate_single\n");
    printk("This test allocates single memory location and checks if correct block is marked as reserved.\n\n");
    
    // Assume memory location. Block before and after should be free.
    // Right now a known good location is few blocks after kernel and physical memory manager 
    // map end.

    unsigned int loc = pmm_get_memory_start();
    loc += PMM_BLOCK_SIZE * 3;
    
    unsigned int loc_free_before = loc - PMM_BLOCK_SIZE;
    unsigned int loc_free_after = loc + PMM_BLOCK_SIZE;

    int block_test = pmm_location_to_block(loc);
    int block_before = pmm_location_to_block(loc_free_before); 
    int block_after = pmm_location_to_block(loc_free_after);

    printk("\tLocation under test: %d (block %d)\n", loc, block_test);
    printk("\tPreceding free location: %d (block %d)\n", loc_free_before, block_before);
    printk("\tFollowing free location: %d (block %d)\n", loc_free_after, block_after);

    if(block_test - block_before != 1) {
        panic("Free block before current block is not immediatelly before current block (huh?)\n");
    }

    if(block_test - block_after != -1) {
        panic("Free block after current block is not immediatelly after current block (huh?)\n");
    }

    extern int pmm_used_blocks, pmm_total_blocks;;
    int used_blocks = pmm_used_blocks;

    printk("Used vs total blocks: %d/%d\n", pmm_used_blocks, pmm_total_blocks);
    // Reserve few blocks 
    int i = pmm_reserve_next_free();
    printk("First allocated block: %d\n", i);
    if(i != 0) panic("First allocated block should be 0.\n");

    i = pmm_reserve_next_free();
    printk("Second allocated block: %d\n", i);  
    if(i != 1) panic("Second allocated block should be 1.\n");
  
 
    pmm_free_block(1);
    i = pmm_next_free();
    if(i != 1) panic("After two block allocations and one release, next one free should be block #1");

    pmm_free_block(0);
    i = pmm_next_free();
    if(i != 0) panic("After two block allocations and two releases, next free block should be block #0");
    
    printk("*******************************\n");
}

void test_allocate_all() {
    printk("*******************************\n");
    printk("TEST: allocate_all\n");
    printk("This test allocates all blocks end ensures all blocks are maked as used.\n\n");

    int free = -1;
    for(int i = 0; i < 161; i++) {
        free = pmm_reserve_next_free();
        printk("%d ", free);
    }
    printk("\n");

    printk("*******************************\n");
}

void test_map_protected() {
    printk("*******************************\n");
    printk("TEST: test_map_protected\n");
    printk("Memory map should live in a block that has been marked as reserved. Check that here.\n\n");

    // Grab location of memory map;
    pmm_reserve_block(0);
    unsigned int maploc = pmm_get_memory_start();
    printk("Start of memory map: %d\n", maploc);
    int blk = pmm_location_to_block(maploc);
    printk("Starting block of map: %d\n", blk);
    printk("Status of starting block of map: %d\n", pmm_block_status(maploc));

    int status = pmm_block_status(blk);
    if(status == 0) panic("Memory map's block should be reserved.");
} 
