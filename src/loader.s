; Entry point for multiboot kernel
global _stack_bottom
global _stack_top

mboot_align equ  1<<0
mboot_meminfo equ  1<<1
mboot_flags equ mboot_align | mboot_meminfo
mboot_magic equ 0x1BADB002
mboot_checksum equ -(mboot_magic + mboot_flags)

;----------------------------------------------------------------------
; Multiboot header (shout be within first 8k, must be 32 bit aligned)
section .multiboot
align 4
dd mboot_magic
dd mboot_flags
dd mboot_checksum


;----------------------------------------------------------------------
; Stack
section .earlystack
align 4
_stack_bottom:
times 1048576 dd 0
_stack_top:

;----------------------------------------------------------------------
; This is entry point
section .text
global _start
_start:

    push ebx
    ; Call kernel and see what's up
    extern kmain
    call kmain 

; Once kernel is done executing, just hang.
.hang:
    hlt
    jmp .hang
