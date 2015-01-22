#include "console.h"
#include "idt.h"
#include "irq.h"
#include "kbd.h"
#include "keymap.h"
#include "sys.h"

extern void isrkbd();
unsigned short *keymap = uskbd;  

void kbd_handler() {
    unsigned char inbyte = inportb(0x60);
    int idx = 0;
    static int shiftkey = 0;
    unsigned short key = 0;

    if(inbyte < 0x80) {
        // key down
        idx = KEYMAP_COLS * inbyte;
        if (shiftkey)
            idx++;
        key = keymap[idx];
        if (key == LSHIFT || key == RSHIFT) {
            shiftkey = 1;
        } else if (key != 0) {
            printk("%c", key);
            //console_printchr(key);
        }         

    } else {
        inbyte -= 0x80;  
        key = keymap[KEYMAP_COLS * inbyte];  
        if (key == LSHIFT || key == RSHIFT)  
            shiftkey = FALSE;
    }

    irq_ack(KBD_IRQ);
}

void init_kbd(void) {
    setup_idt_gate(KBD_IRQ, isrkbd);        
}


