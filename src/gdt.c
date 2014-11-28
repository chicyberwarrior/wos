#include "console.h"
#include "gdt.h"


struct gdt_segment_descriptor gdt_entries[NUM_GDT_ENTRIES];
struct gdt_pointer gdt_ptr;

void setup_gdt_segment_descriptor_bits(int i, unsigned int base_addr, unsigned int size, unsigned char sec, unsigned char granularity) {

    gdt_entries[i].seg_size = (size & 0xFFFF);
    gdt_entries[i].base_addr_1 = (base_addr & 0xFFFF);
    gdt_entries[i].base_addr_2 = (base_addr >> 16) & 0xFF;
    gdt_entries[i].base_addr_3 = (base_addr >> 24) & 0xFF;
    gdt_entries[i].seg_sec = sec;
    gdt_entries[i].seg_granularity = (size >> 16) & 0x0F;
    gdt_entries[i].seg_granularity |= granularity & 0xF0;
   
}

void setup_gdt_segment_descriptor(int entryn, unsigned int base, unsigned char sec, unsigned char gran) {
    setup_gdt_segment_descriptor_bits(entryn, 0, base, sec, gran); 
}

void flush_gdt() {
    _flush_gdt_and_refresh();
}

void setup_gdt() {
    unsigned int gdt_base = (unsigned int) &gdt_entries;
    unsigned short gdt_size = (unsigned short) sizeof(struct gdt_segment_descriptor) * NUM_GDT_ENTRIES;
    
    gdt_ptr.size = gdt_size;
    gdt_ptr.base = gdt_base;

    printk("GDT base address: 0x%x, size: %d\n", gdt_ptr.base, gdt_ptr.size);

    // This is messy. See on-line resources for more explanation:
    //  http://www.jamesmolloy.co.uk/tutorial_html/4.-The%20GDT%20and%20IDT.html
    //
    // I set up segment to overlap and take whole memory space.
    // I also could do separate segments, but not sure if I need it right now.
    //  
    // 0xCF - 0xF is for size, 
    //      0xC = 1100b
    //      1 - size of segment granularity. 0 for bytes, 1 for 4kB pages
    //          Since we provide 0xFFFFF for size, it means 4kb * 0xFFFFF
    //          Size is 20 bits, so minimum segment size is 
    //              2^20 * 1 byte = 1 MB
    //          and maximum segment size is
    //              2^20 * 4kB = 4 GB
    //      1 - D - word size - 0 for 16 bits, 1 for 32 bits.
    //      0 - always 0
    //      0 - AVl - always 0
    // 0x9A - 10011010b
    //      P - segment present (yes = 1)
    //      D - 2 bits - privilege level - 00 for ring 0 (yay!)
    //      S - type of segment - 0 ldt/tss/etc, 1 data/text
    //      type - 
    //          bit 0 - executable (1/0)
    //          bit 1 - expansion (data seg),confirming (code seg)
    //          bit 2 - read/write (for data segments: 0=RO, 1=RW) (for code segments: 0=Execute only, 1=Read/execute) 
    //      A - access bit - use 0
    //      
    //      So, this value means: present, ring 0, system seg, executabe/confirming, not accessed
    //
    // 0x9A - 10011010b - code segment
    // 0x92 - 10010010 - data segment
    // 0xFA - 11111010 - user mode code 
    // 0xF2 - 11110010 - user mode data

    setup_gdt_segment_descriptor(0, 0, 0, 0);
    setup_gdt_segment_descriptor(1, 0xFFFFFFFF, 0x9A, 0xCF); // code 
    setup_gdt_segment_descriptor(2, 0xFFFFFFFF, 0x92, 0xCF); // data
    setup_gdt_segment_descriptor(3, 0xFFFFFFFF, 0xFA, 0xCF); // user mode code
    setup_gdt_segment_descriptor(4, 0xFFFFFFFF, 0xF2, 0xCF); // user mode data

    flush_gdt();    
}
