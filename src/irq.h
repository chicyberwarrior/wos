#ifndef irq_h
#define irq_h

struct irq_registers {
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
    unsigned int irq;
    unsigned int err;
};

void remap_pic();
void setup_irq_gates();
void default_irq_handler(struct irq_registers regs);
void maskIRQ(unsigned char irq);
void unmaskIRQ(unsigned char irq);
#endif
