#include "irq.h"
#include "sys.h"
#include "console.h"
#include "idt.h"

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

void remap_pic() {
    outportb(0x20, 0x11);
    outportb(0xA0, 0x11);

    outportb(0x21, 0x20);
    outportb(0xA1, 0x28);

    outportb(0x21, 0x04);
    outportb(0xA1, 0x02);

    outportb(0x21, 0x01);
    outportb(0xA1, 0x01);
    
    // kbd only
    outportb(0x21,0xfc);
    outportb(0xa1,0xff);

    //outportb(0x21, 0x0);
    //outportb(0xA1, 0x0);

}

void setup_irq_gates() {
    setup_idt_gate(32, irq0);
    setup_idt_gate(33, irq1);
    setup_idt_gate(34, irq2);
    setup_idt_gate(35, irq3);
    setup_idt_gate(36, irq4);
    setup_idt_gate(37, irq5);
    setup_idt_gate(38, irq6);
    setup_idt_gate(39, irq7);
    setup_idt_gate(40, irq8);
    setup_idt_gate(41, irq9);
    setup_idt_gate(42, irq10);
    setup_idt_gate(43, irq11);
    setup_idt_gate(44, irq12);
    setup_idt_gate(45, irq13);
    setup_idt_gate(46, irq14);
    setup_idt_gate(47, irq15);
}

void irq_ack(unsigned int irq_no) {
    if (irq_no >= 12) {
        outportb(0xA0, 0x20);
    }
    outportb(0x20, 0x20);
}

void default_irq_handler(struct irq_registers regs) {
    
    // Timer
    if(regs.irq == 0x20) {
        outportb(0x20, 0x20);
        outportb(0xa0,0x20);
        return;
    }

    // Keyboard
    if(regs.irq == 0x21) {
        printk("KEY\n");
        inportb(0x60);
    }
   
    irq_ack(regs.irq); 
    //outportb(0x20, 0x20);
    //outportb(0xa0,0x20);
    //maskIRQ(0x0);
    return;

    //irq_ack(irqno);
}

void maskIRQ(unsigned char irq)
{
    if(irq==0xFF)
    {
        outportb(0x21,0xFF);
        outportb(0xA1,0xFF);
    }
    else
    {
        irq = irq | (1<<irq);
        if(irq < 8)
            outportb(0x21, irq&0xFF);
        else
            outportb(0xA1, irq>>8);
    }
}

void unmaskIRQ(unsigned char irq)
{
    if(irq==0xFF)
    {
        outportb(0x21,0x00);
        outportb(0xA1,0x00);
    }
    else
    {
        irq = irq & (1<<irq);
        if(irq < 8)
            outportb(0x21, irq&0x00);
        else
            outportb(0xA1, irq>>8);
    }
}



