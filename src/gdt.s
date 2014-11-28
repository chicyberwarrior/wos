; Dada Kernel
; October 2014
;
; Assembler helpers for GDT initialization.
;
; Wiktor Lukasik (wiktor@lukasik.org)
 
global _flush_gdt_and_refresh

[extern gdt_ptr]
_flush_gdt_and_refresh:
    lgdt [gdt_ptr]
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:flush2
flush2:
    ret
