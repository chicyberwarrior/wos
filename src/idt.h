#ifndef idt_h
#define idt_h

#include "types.h"

#define IDT_NUM_ENTRIES 256

typedef struct registers
{
    unsigned int ds;
    unsigned edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int int_no, err_code;
    unsigned int eip, cs, eflags, useresp, ss;
} registers_t;

struct idt_entry {
   uint16_t base_low;
   uint16_t seg_sel;
   uint8_t gap; 
   uint8_t flags;
   uint16_t base_high;
} __attribute__((__packed__));

struct idt_entry idt_entries[IDT_NUM_ENTRIES];

struct idt_desc {
    uint16_t idt_size;
    uint32_t idt_addr;
} __attribute__((__packed__));

void clear_idt();
void flash_idt();
void setup_idt();
void setup_idt_gate(int gate, void (*func_base)());

#endif
