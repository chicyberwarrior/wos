/**
 * Dada Kernel
 * October 2014
 *
 * This file is the entry point for the kernel. Called from loader.s.
 * Responsible for initialization of stuff, like interrupt handling, memory and such.
 *
 * Wiktor Lukasik (wiktor@lukasik.org)
 */

/* This is kernel. */
#include "console.h"
#include "types.h"
#include "string.h"
#include "idt.h"
#include "gdt.h"
#include "irq.h"
#include "sys.h"
#include "physmem.h"
#include "experiment/current/experiment.h"

extern unsigned int * kernel_begin;
extern unsigned int * kernel_end;

extern int * _stack_bottom;
extern int * _stack_top;
extern char cpu_name[15];

void show_memory_map(struct mbootinfo * bootInfo)
{
    int i,parts;
    printk("Memory map: \n");
    parts=((int)bootInfo->mmap_length)/sizeof(memoryMap);
    unsigned int total_size = 0;

    for (i=0;i<parts;i++) {
        unsigned long size = bootInfo->mmap_addr[i].LengthLow;
        total_size += size;
    }

    pmm_initialize((unsigned int) &kernel_end, total_size);

    for (i=0;i<parts;i++) {
        unsigned long start = bootInfo->mmap_addr[i].BaseAddrLow;
        unsigned long size = bootInfo->mmap_addr[i].LengthLow;
        unsigned long end = start + size;

        printk("  0x%x - 0x%x (%d bytes)", start, end, size);

        if(1 == bootInfo->mmap_addr[i].Type) {
            printk(" [usable]");
            pmm_add_memory_region(start, end);
        } else {
            printk(" [reserved]");
        }
        printk("\n");
    }

    pmm_add_memory_region((unsigned int) &kernel_begin, (unsigned int) &kernel_end);
    pmm_add_memory_region((unsigned int) &kernel_end, (unsigned int) &kernel_end + total_size / PMM_BLOCK_SIZE / 8);
    printk("Total found usable memory: %d bytes\n", pmm_get_memory_size());
}

void kmain(int * s)
{   
    unsigned int kernel_size = &kernel_end - &kernel_begin;

    /* Point multi boot info structure to whatever is at EBX (set up by loader) */
    struct mbootinfo * bi = (struct mbootinfo *) s;

    console_cls();
    cpuid();
    printk("Dada v0.0.1 | CPU:  %s | %u | 0x%x - 0x%x\n", cpu_name, kernel_size, &kernel_begin, &kernel_end);
    show_memory_map(bi);
    printk("Stack: 0x%x - 0x%x (%d bytes)\n", &_stack_bottom, &_stack_top, &_stack_top - &_stack_bottom);
    printk("BIOS reported lower memory: %u KB\n", bi->memlow );
    printk("BIOS reported higher memory: %u KB\n", bi->memhigh );
     
    setup_gdt();
    setup_idt();
    remap_pic();
    setup_irq_gates();
    flash_idt();

    pmm_print_summary();

    unmaskIRQ(1);
    asm volatile("sti");
    //asm volatile("int $0x1");
    
    printk("===================================\n");
    //experiment();
    printk("===================================\n");

    //pmm_set_block(0, 1);
    //pmm_set_block(2, 1);
    //pmm_dump_mm();
}


