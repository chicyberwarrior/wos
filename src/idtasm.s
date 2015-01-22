extern default_handler
extern default_irq_handler
extern kbd_handler

%macro ISR_DEFAULT 1
global isr%1
isr%1:
    push 0
    push %1
    pushad
    push ds
    push es
    push fs
    push gs
    call default_handler
    pop gs
    pop fs
    pop es
    pop ds
    popad
    add esp, 8
    iret
%endmacro

global isrkbd
isrkbd:
    push 0
    push 33
    pushad
    push ds
    push es
    push fs
    push gs
    call kbd_handler
    pop gs
    pop fs
    pop es
    pop ds
    popad
    add esp, 8
    iret

%macro ISR_ERR 1
global isr%1
isr%1:
    push 0
    push %1
    pushad
    push ds
    push es
    push fs
    push gs
    call default_handler
    pop gs
    pop fs
    pop es
    pop ds
    popad
    add esp, 8
    iret
%endmacro


ISR_DEFAULT 0
ISR_DEFAULT 1
ISR_DEFAULT 2
ISR_DEFAULT 3
ISR_DEFAULT 4
ISR_DEFAULT 5
ISR_DEFAULT 6
ISR_DEFAULT 7
ISR_ERR 8
ISR_DEFAULT 9
ISR_ERR 10
ISR_ERR 11
ISR_ERR 12
ISR_ERR 13
ISR_ERR 14
ISR_DEFAULT 15
ISR_DEFAULT 16
ISR_DEFAULT 17
ISR_DEFAULT 18
ISR_DEFAULT 19
ISR_DEFAULT 20
ISR_DEFAULT 21
ISR_DEFAULT 22
ISR_DEFAULT 23
ISR_DEFAULT 24
ISR_DEFAULT 25
ISR_DEFAULT 26
ISR_DEFAULT 27
ISR_DEFAULT 28
ISR_DEFAULT 29
ISR_DEFAULT 30
ISR_DEFAULT 31

%macro IRQ_DEFAULT 2
global irq%1
irq%1:
    push 0
    push %2
    pushad
    push ds
    push es
    push fs
    push gs
    call default_irq_handler
    pop gs
    pop fs
    pop es
    pop ds
    popad
    add esp, 8

    ;push eax
    ;push edx
    ;xor eax, eax
    ;xor edx, edx
    ;mov edx, 0x20
    ;mov eax, edx
    ;out dx, eax
   ; pop edx
    ;pop eax
    iret
%endmacro

IRQ_DEFAULT 0, 32
IRQ_DEFAULT 1, 33
IRQ_DEFAULT 2, 34
IRQ_DEFAULT 3, 35
IRQ_DEFAULT 4, 36
IRQ_DEFAULT 5, 37
IRQ_DEFAULT 6, 38
IRQ_DEFAULT 7, 39
IRQ_DEFAULT 8, 40
IRQ_DEFAULT 9, 41
IRQ_DEFAULT 10, 42
IRQ_DEFAULT 11, 43
IRQ_DEFAULT 12, 44
IRQ_DEFAULT 13, 45
IRQ_DEFAULT 14, 46
IRQ_DEFAULT 15, 47

