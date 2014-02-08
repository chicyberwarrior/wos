#!/bin/sh

nasm -g -f bin stage1/stage1.s -o data/stage1.bin
sudo ./scripts/mnt.sh
sudo cp -v data/stage1.bin /media/floppy/
sleep 1
sudo umount /media/floppy
