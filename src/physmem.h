#ifndef physmem_h
#define physmem_h

void pmm_set_memory_size(unsigned int size);
unsigned int pmm_get_memory_size();

void pmm_initialize(unsigned int where, int size);
void pmm_add_memory_region(unsigned int start, unsigned int end);
void pmm_remove_memory_region(unsigned int start, unsigned int end);

void pmm_set_block(int bit, int bal);
int pmm_get_block(int bit);

void pmm_print_summary();

/* Usable memory regions */
struct UsableMemoryRegion {
    unsigned int begin;
    unsigned int end;
};

#endif
