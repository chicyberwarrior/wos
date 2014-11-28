/**
 * Dada Kernel
 * October 2014
 *
 * Contains types and functions of general sort, used by the kernel.
 *
 * Wiktor Lukasik (wiktor@lukasik.org)
 */

#ifndef sys_h
#define sys_h

/* Contains name of the CPU as reported by cpuid. Populated by void cpuid(); */
char cpu_name[15];

typedef struct memoryMap
{
  unsigned long Size;
  unsigned long BaseAddrLow;
  unsigned long BaseAddrHigh;
  unsigned long LengthLow;
  unsigned long LengthHigh;
  unsigned long Type;
} __attribute__ ((packed)) memoryMap;

/* Flags and memory range passed in by multiboot compatibile bootloader */
struct mbootinfo {
    unsigned long flags;
    unsigned long memlow;
    unsigned long memhigh;
    unsigned char bootDevice[4];
    char *commandLine;
    unsigned long modulesCount;
    unsigned int *modules;
    unsigned long num;
    unsigned long size;
    unsigned long addr;
    unsigned long reserved;
    unsigned long mmap_length;
    memoryMap *mmap_addr;
    unsigned long drives_length;
    unsigned long drives_addr;
    unsigned long config_table;
    unsigned long bootloader_name;
};

/* Executes cpuid instruction and populates cpu_name with name of the processor. */
void cpuid();

/* Writes data to port */
void outportb(unsigned short _port, unsigned char _data);

/* Reads data from a port */
unsigned char inportb(unsigned short _port);


#endif
