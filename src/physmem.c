#include "console.h"
#include "string.h"
#include "physmem.h"

#define PMM_BLOCK_SIZE 4096

unsigned int pmm_total_memory_size = 0;
unsigned int * pmm_memory_map = 0;
int pmm_mem_size = 0;
int pmm_total_blocks = 0;
int pmm_used_blocks = 0;

void pmm_initialize(unsigned int where, int size) {
    pmm_memory_map = (unsigned int *) where;
    pmm_mem_size = size;
    pmm_total_blocks = pmm_mem_size / PMM_BLOCK_SIZE;
    pmm_used_blocks = pmm_total_blocks;
    memset((unsigned int *)where, 0xFFFFFFFF, pmm_mem_size / PMM_BLOCK_SIZE / 8);
}

void pmm_set_memory_size(unsigned int size) {
    pmm_total_memory_size = size;
}

unsigned int pmm_get_memory_size() {
    return pmm_total_memory_size;
}

void pmm_add_memory_region(unsigned int start, unsigned int end) {
    pmm_total_memory_size += end - start;

    int bit = start / PMM_BLOCK_SIZE;
    int num = (end - start) / PMM_BLOCK_SIZE;

    //printk("*** %d %d\n", bit, num);
    if((end - start) % PMM_BLOCK_SIZE != 0) {
        num++;
    }

    for(int i = 0; i < num; i++) {
        //printk("Seting block %d\n", bit);
        pmm_set_block(bit++, 0);
        pmm_used_blocks--;
    }
}

void pmm_set_block(int bit, int val) {
    //printk(">0x%x<\n", &pmm_memory_map[bit / 32]);
    if(val) {
        pmm_memory_map[bit / 32] |= (1 << (bit % 32));
    } else {
        pmm_memory_map[bit / 32] &= ~ (1 << (bit % 32));
    }
}

int pmm_get_block(int bit) {
    return (pmm_memory_map[bit / 32] &  (1 << (bit % 32))) != 0;
}

void pmm_print_summary() {
    printk("PMM internal summary:\n");
    printk("    pmm_mem_size: %d\n    pmm_total_blocks: %d\n    pmm_used_blocks: %d\n", 
        pmm_mem_size, pmm_total_blocks, pmm_used_blocks);
}
