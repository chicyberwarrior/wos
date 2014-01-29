#!/bin/sh

# 1. Create new floppy image
# 2. Write stage0 to boot sector
# 3. Mount floppy 
# 4. Copy file
# 5. Unmount

echo "Building stage0..."
./scripts/build.sh
echo "Creating new floppy..."
./scripts/mkfloppy.sh

if [ ! -f data/new_floppy.img ]
    then
        echo "Did not find new floppy image. Exiting."
        exit 1
fi

echo "Skipping first 512 bytes..."
dd if=data/new_floppy.img of=data/new_floppy_data.img bs=1 skip=512

if [ ! -f data/new_floppy_data.img ]
    then
        echo "Failed to find data file."
        exit 2 
fi

echo "Creating floppy image..."
cat data/stage0.bin data/new_floppy_data.img > data/floppy.img

echo "Mounting floppy..."
sudo mount -t msdos -o loop,fat=12 data/floppy.img /media/floppy

echo "Copying stage1..."
sudo cp data/stage1.bin /media/floppy

sleep 2

echo "Unmounting floppy..."
sudo umount /media/floppy


echo "DONE!"
