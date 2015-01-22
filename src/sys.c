/**
 * Dada Kernel
 * October 2014
 *
 * Wiktor Lukasik (wiktor@lukasik.org)
 */

#include "console.h"
#include "sys.h"

void cpuid() {
        register int b asm("ebx");
        register int c asm("ecx");
        register int d asm("edx");

        __asm__("movl $0, %eax");
        __asm__("cpuid");

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

void outportb(unsigned short port, unsigned char data) {
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (data));
}

unsigned char inportb(unsigned short port) {
    unsigned char rv;
    asm volatile ("inb %1, %0" : "=a" (rv) : "dN" (port));
    return rv;
}

void panic(char * msg) {
    if(msg != 0) {
        printk("PANIC MESSAGE: %s\n", msg);
    }

    halt_and_catch_fire();
}

void halt_and_catch_fire() {
    printk("SYSTEM HALTED!");
    // TODO: come up with a more definitive kill method.
    for(;;) {}
}
