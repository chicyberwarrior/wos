#include "types.h"
#include "idt.h"
#include "string.h"
#include "console.h"

extern struct idt_entry idt_entries[IDT_NUM_ENTRIES];

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

void flash_idt() {
    struct idt_desc idtdesc;

    idtdesc.idt_size = sizeof(struct idt_entry) * IDT_NUM_ENTRIES;
    idtdesc.idt_addr = (uint32_t) &idt_entries;

    asm volatile("lidt %0" : : "m" (idtdesc) : "memory");
}

void setup_idt_gate(int gate, void (*func_base)()) {
    unsigned int base = (unsigned int) func_base;
    idt_entries[gate].base_low = base & 0xFFFF;
    idt_entries[gate].seg_sel= 0x08;
    idt_entries[gate].gap = 0;
    idt_entries[gate].flags = 0x8E | 0x60;
    idt_entries[gate].base_high = (base >> 16) & 0xFFFF;;

}

/*
void x(registers_t r) {
    console_print("INTERRUPT "); console_printnum(r.int_no); console_print("\n");
    asm volatile("cli");
    console_print("INT INT INT\n");
    //asm volatile("sti");
    //asm volatile("iret");
};
*/

struct isr_registers {
    unsigned int gs;
    unsigned int fs;
    unsigned int es;
    unsigned int ds;
    unsigned int edi;
    unsigned int esi;
    unsigned int ebp;
    unsigned int esp;
    unsigned int ebx;
    unsigned int edx;
    unsigned int ecx;
    unsigned int eax;
    unsigned int isr;
    unsigned int err;

};

void print_registers(struct isr_registers regs) {
   console_print("GS: "); console_printhex(regs.gs);
   console_print(" FS: "); console_printhex(regs.fs);
   console_print(" ES: "); console_printhex(regs.es);
   console_print(" DS: "); console_printhex(regs.ds);
   console_print(" EDI: "); console_printhex(regs.edi);
   console_print(" ESI: "); console_printhex(regs.esi);
   console_print(" EBP: "); console_printhex(regs.ebp);
   console_print(" ESP: "); console_printhex(regs.esp);
   console_print(" EBX: "); console_printhex(regs.ebx);
   console_print(" EDX: "); console_printhex(regs.edx);
   console_print(" ECX: "); console_printhex(regs.ecx);
   console_print(" EAX: "); console_printhex(regs.eax);
   console_print("\n");
}

void default_handler(struct isr_registers args) { // registers_t r) {
    console_print(">>> INTERRUPT: "); console_print(" INT# "); console_printhex(args.isr); 
    console_print(" ERR# "); console_printhex(args.err); console_print("\n");
    print_registers(args);
    //if(args.isr == 13) {
    //    int x = 3/0;
    //    int y = 3+x;
    //}
    //if(args.isr == 0) {
    //    outportb(0x20, 0x20);
    //}
    //asm volatile("cli");
    //console_print("INTERRUPT ");// console_printnum(r.int_no); console_print("\n");
    //console_print("DEFAULT ITERRUPT HANDLER!!!\n");
    //asm volatile("sti");
    //asm volatile("iret");
}

//void setup_idt() {
    //setup_idt_gate(3, (unsigned int) x);
//}

void setup_idt() {
    memset(&idt_entries, 0, sizeof(struct idt_entry) * IDT_NUM_ENTRIES);
    
    setup_idt_gate(0, &isr0);
    setup_idt_gate(1, &isr1);
    setup_idt_gate(2, &isr2);
    setup_idt_gate(3, &isr3);
    setup_idt_gate(4, &isr4);
    setup_idt_gate(5, &isr5);
    setup_idt_gate(6, &isr6);
    setup_idt_gate(7, &isr7);
    setup_idt_gate(8, &isr8);
    setup_idt_gate(9, &isr9);
    setup_idt_gate(10, &isr10);
    setup_idt_gate(11, &isr11);
    setup_idt_gate(12, &isr12);
    setup_idt_gate(13, &isr13);
    setup_idt_gate(14, &isr14);
    setup_idt_gate(15, &isr15);
    setup_idt_gate(16, &isr16);
    setup_idt_gate(17, &isr17);
    setup_idt_gate(18, &isr18);
    setup_idt_gate(19, &isr19);
    setup_idt_gate(20, &isr20);
    setup_idt_gate(21, &isr21);
    setup_idt_gate(22, &isr22);
    setup_idt_gate(23, &isr23);
    setup_idt_gate(24, &isr24);
    setup_idt_gate(25, &isr25);
    setup_idt_gate(26, &isr26);
    setup_idt_gate(27, &isr27);
    setup_idt_gate(28, &isr28);
    setup_idt_gate(29, &isr29);
    setup_idt_gate(30, &isr30);
    setup_idt_gate(31, &isr31);
}

