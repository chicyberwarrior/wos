.macro ISR n
.global isr\n
isr\n:
    cli 
    push $\n
    jmp isr_common
.endm

ISR 0

isr_common:
    pusha

    call x

    popa
    sti
    iret
