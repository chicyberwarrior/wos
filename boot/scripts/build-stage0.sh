#!/bin/sh

nasm -g -f bin stage0/boot.s -o data/stage0.bin
cat data/stage0.bin data/new_floppy_data_master.img > data/floppy.img
