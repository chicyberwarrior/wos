#ifndef io_h
#define io_h

static inline void outb(unsigned int port, unsigned char val)
{
    asm volatile ( "outb %%al, %%dx" : : "d"(port), "a"(val));
}

#endif
