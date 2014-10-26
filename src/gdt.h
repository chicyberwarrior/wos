#ifndef gdt_h
#define gdt_h

#define NUM_GDT_ENTRIES 5
/**
 * GDT segment descriptor.
 * http://en.wikipedia.org/wiki/Global_Descriptor_Table
 * http://wiki.osdev.org/GDT_Tutorial
 * Thing to remember is that last 4 bits of segment size is in seg_granularity field.
 */
struct gdt_segment_descriptor {
	unsigned short seg_size;
	unsigned short base_addr_1;
	unsigned char base_addr_2;
	unsigned char seg_sec;
	unsigned char seg_granularity;
	unsigned char base_addr_3;
} __attribute__((__packed__));


/** Pointer to GDT used by LGDT 
 * word  - size (2 bytes)
 * dword - base (4 bytes)
 */
struct gdt_pointer {
    unsigned short size;
    unsigned int base;
} __attribute__((__packed__));

/* Build GDT entry */
void setup_gdt_segment_descriptor(int entryn, unsigned int base, unsigned char sec, unsigned char gran);

/* Install GDT */
void setup_gdt();

/* Writes GDT and regreshes segment registers. In ASM. */
void _flush_gdt_and_refresh();
#endif
