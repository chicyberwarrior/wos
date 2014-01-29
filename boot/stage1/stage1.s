org 0x7e00
bits 16

jmp stage1

%include '../stage0/util.s'

hello db "Hello", 0

stage1:

push PRINTNL
mov si, hello
call printn
add sp, 2

