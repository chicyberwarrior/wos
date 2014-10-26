#include "types.h"
#include "idt.h"
#include "string.h"
#include "console.h"

extern struct idt_entry idt_entries[IDT_NUM_ENTRIES];

void flash_idt(void * idt_ptr) {
    struct idt_desc idtdesc;

    idtdesc.idt_size = sizeof(struct idt_entry) * IDT_NUM_ENTRIES;
    idtdesc.idt_addr = &idt_entries;

    asm volatile("lidt %0" : : "m" (idtdesc) : "memory");
}

void setup_idt_gate(int gate, unsigned int base) {
    idt_entries[gate].base_low = base & 0xFFFF;
    idt_entries[gate].seg_sel= 0x08;
    idt_entries[gate].gap = 0;
    idt_entries[gate].flags = 0x8E;
    idt_entries[gate].base_high = (base >> 16) & 0xFFFF;;

}

void x(registers_t r) {
    console_print("INTERRUPT "); console_printnum(r.int_no); console_print("\n");
    asm volatile("cli");
    console_print("INT INT INT\n");
    //asm volatile("sti");
    //asm volatile("iret");
};

void default_handler(registers_t r) {
    asm volatile("cli");
    console_print("INTERRUPT "); console_printnum(r.int_no); console_print("\n");
    console_print("DEFAULT ITERRUPT HANDLER!!!\n");
}

void setup_idt() {
    setup_idt_gate(3, (unsigned int) x);
}

void clear_idt() {
    int i;

    memset(&idt_entries, 0, sizeof(struct idt_entry) * IDT_NUM_ENTRIES);

    for(i = 0; i < IDT_NUM_ENTRIES; i++) {
        setup_idt_gate(i, & default_handler);
    }
}

