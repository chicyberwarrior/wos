/* This is kernel. */
#include "console.h"

extern int * _stack_bottom;
extern int * _stack_top;

char cpu_name[15];

/* Flags and memory range passed in by multiboot compatibile bootloader */
struct mbootinfo {
    int flags;
    int memlow;
    int memhigh;
}; 

/* Get CPU id and store in cpu_name */
void cpuid() {
        register int b asm("ebx");
        register int c asm("ecx");
        register int d asm("edx");

        __asm__("movl $0, %eax");	
        __asm__("cpuid");

        int b1 = b >> 4;

        cpu_name[0] = (char) b;
        cpu_name[1] = (char) (b >> 8);
        cpu_name[2] = (char) (b >> 16);
        cpu_name[3] = (char) (b >> 24);
        cpu_name[4] = (char) (d);
        cpu_name[5] = (char) (d >> 8);
        cpu_name[6] = (char) (d >> 16);
        cpu_name[7] = (char) (d >> 24);
        cpu_name[8] = (char) (c);
        cpu_name[9] = (char) (c >> 8);
        cpu_name[10] = (char) (c >> 16);
        cpu_name[11] = (char) (c >> 24);
        cpu_name[12] = '\0';
}

void dump_registers() {
}

void kmain(int * s)
{   
    /* Point multi boot info structure to whatever is at EBX (set up by loader) */
    struct mbootinfo * bi = s;

    /* Get CPU name */
    cpuid();
    
    /* Print some system info */
    console_cls();
    console_print("Stack: "); 
    console_printhex(&_stack_bottom); 
    console_printchr('-'); 
    console_printhex(&_stack_top); 
    console_print(" ("); 
    console_printnum(&_stack_top - &_stack_bottom); 
    console_print(" bytes)");
    console_printchr('\n');
    console_print("Memory range: ");
    console_printhex((bi->memlow) * 1000);
    console_printchr('-');
    console_printhex((bi->memhigh) * 1000);
    console_print(" (");
    console_printnum(((bi->memhigh) * 1000 - (bi->memlow) * 1000));
    console_print(" bytes)\n");
    console_print("CPU type: ");
    console_print(cpu_name);
    console_printchr('\n');
    console_printchr('\n');
    
    /* Dump contents of registers */
    dump_registers();
}
