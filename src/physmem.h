/**
 * Dada Kernel
 * October 2014
 *
 * Physical memory manager.
 * 
 * 
 *
 * Wiktor Lukasik (wiktor@lukasik.org)
 */

#ifndef physmem_h
#define physmem_h

#include "sys.h"

/* Size of a memory block in bytes. */
#define PMM_BLOCK_SIZE 4096

/* Usable memory regions */
struct UsableMemoryRegion {
    unsigned int begin;
    unsigned int end;
};

/* Initializes physical memory manager. 'where' points to where we want 
to store memory map. 'size' is the total size of memory in bytes. */
void pmm_initialize(unsigned int where, int size);

int pmm_set_block(int bit, int val);
int pmm_get_block(int bit);

/* Set total memory size in bytes. */
void pmm_set_memory_size(unsigned int size);
/* Get total memory size in bytes. */
unsigned int pmm_get_memory_size();
/* Get location of memory map. */
unsigned int pmm_get_memory_start();

/* Add region of usable memory. */
void pmm_add_memory_region(unsigned int start, unsigned int end);
/* Remove a region of memory. */
void pmm_remove_memory_region(unsigned int start, unsigned int end);

/* Find next free block of memory. */
int pmm_next_free();
/* Reserve memory block */
int pmm_reserve_block(int block);
/* Find & reserve next free block of memory. */
int pmm_reserve_next_free();
/* Release a block. */
int pmm_free_block(int block);

/* Check status of block */
int pmm_block_status(int block);
/* Translate location in memory to a block number. */
int pmm_location_to_block(unsigned int loc);
/* Prints small summary of physical memory manager internal state. */
void pmm_print_summary();

/* Diagnostic functions */
void pmm_dump_mm();

#endif
