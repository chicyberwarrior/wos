#ifndef io_h
#define io_h

#define PORT 0x3f8 
#define SERIAL_COM1_BASE                    0x3F8
#define SERIAL_DATA_PORT(base)              (base)
#define SERIAL_FIFO_COMMAND_PORT(base)      (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)      (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base)     (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)       (base + 5)

#define SERIAL_LINE_ENABLE_DLAB             0x80


static inline void outb(unsigned int port, unsigned char val)
{   
    asm volatile("outb %0, %w1" : : "a" (val), "d" (port));
    //asm volatile ( "outb %%al, %%dx" : : "d"(port), "a"(val));
}

static inline unsigned char inb(int port)
{
    unsigned char ret;
    asm volatile ( "inb %w1, %0" : "=a"(ret) : "d"(port) );
    return ret;
}


/* DOCUMENT ALL OF THIS */
void serial_configure_baud_rate(unsigned short com, unsigned short divisor);
void serial_configure_line(unsigned short com);
int serial_is_transmit_fifo_empty(unsigned int com);
void init_serial();
int is_transmit_empty();

#endif
