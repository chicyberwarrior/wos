org 0x0
bits 16

jmp stage1

%include 'stage0/util.s'

msg_enablinga20 db "Enabling gate A20", 0
msg_enableda20 db "Enabled gate A20", 0

msg_criticalerror db "Critical error. Halting!", 0
stage1:
    ; Stage 1 located at 0x7e00
    mov ax, 0x7e0
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Enable gate A20
    push PRINTNL
    mov si, msg_enablinga20
    call printn
    add sp, 2
    mov ax, 0x2401
    int 0x15
    
    jc critical_error
    jmp halt
    
; Inform of unrecoverable error
critical_error:
    push PRINTNL
    mov si, msg_criticalerror
    call printn
    add sp, 2

; Halt
halt:
    hlt
    jmp halt

