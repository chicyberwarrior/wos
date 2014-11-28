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

        printk("  0x%x - 0x%x (%d bytes) ", start, end, size);
/*
        console_print("  ");
        console_printhex(bootInfo->mmap_addr[i].BaseAddrHigh);
        console_printhex(bootInfo->mmap_addr[i].BaseAddrLow);
        console_print(" len:");
        console_printhex(bootInfo->mmap_addr[i].LengthHigh);
        console_printhex(bootInfo->mmap_addr[i].LengthLow);
*/  
        if(1==bootInfo->mmap_addr[i].Type) {
            console_print(" [usable]");
            pmm_add_memory_region(start, end);
        } else {
            console_print(" [reserved]");
        }
        console_print("\n");
    }

    printk("Total found usable memory: %d bytes\n", pmm_get_memory_size());
}

void kmain(int * s)
{   
    unsigned int kernel_size = &kernel_end - &kernel_begin;

    /* Point multi boot info structure to whatever is at EBX (set up by loader) */
    struct mbootinfo * bi = (struct mbootinfo *) s;
    console_cls();
    
    cpuid();
    printk("Dada v0.0.1\n");
    printk("Running on %s\n", cpu_name);
    printk("Kernel size: %u\n", kernel_size);
    printk("Kernel range: 0x%x - 0x%x\n\n", &kernel_begin, &kernel_end);

    show_memory_map(bi);

    /* Print some system info */

    /* Dump contents of registers */
    console_print("Stack: "); 
    console_printhex((int) &_stack_bottom);
    console_printchr('-'); 
    console_printhex((int) &_stack_top);
    console_print(" ("); 
    console_printnum(&_stack_top - &_stack_bottom); 
    console_print(" bytes)");
    console_printchr('\n');

    printk("BIOS reported lower memory: %u KB\n", bi->memlow );
    printk("BIOS reported higher memory: %u KB\n", bi->memhigh );
     
    setup_gdt();
    printk("GDT installed.\n");
    setup_idt();
    printk("IDT installed.\n");
    remap_pic();
    printk("PIC remapped.\n");
    setup_irq_gates();
    flash_idt();
    printk("IRQ handlers installed.\n");
    asm volatile("sti");
    printk("Interrupts enabled!\n");

    pmm_print_summary();
    //unmaskIRQ(0x08);
    //asm volatile("sti");  

    /*asm volatile("int $0x4");
    asm volatile("int $0x6");
    asm volatile("int $0x5");
    asm volatile("int $0x7");
    asm volatile("int $0x3");
    asm volatile("int $0xD");
    asm volatile("int $0xD");
    asm volatile("int $0xD");
    asm volatile("int $0xD");
    asm volatile("int $0xD");
    asm volatile("int $0x3");
    asm volatile("int $0x2");
    asm volatile("int $0x1");*/
    //asm volatile("int $0xbd");
    //asm volatile("int $0xaf");
    //int x = 2/0;
}
