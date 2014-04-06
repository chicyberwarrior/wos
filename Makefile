ASM_CMD = nasm
ASM_FLAGS = -felf
LD_CMD = ld
GCC_CMD = gcc
CC_CUST_FLAGS = -g
CC_FLAGS = -Wall -Wextra -nostdlib -fno-builtin -nostartfiles -nodefaultlibs

BUILD_DIR = build
SRC_DIR = src
SCRIPT_DIR = scripts
LINKER_DIR = src
LINKER_SCRIPT = linker.ld
KERNEL_IMAGE = kernel.bin

.PHONY: all

all: clean kernel 

kernel: kernel.o loader.o 
	$(LD_CMD) -T  $(LINKER_DIR)/$(LINKER_SCRIPT) -o $(BUILD_DIR)/$(KERNEL_IMAGE) $(SRC_DIR)/loader.o $(SRC_DIR)/kernel.o

kernel.o:
	$(GCC_CMD) -o $(SRC_DIR)/kernel.o -c $(SRC_DIR)/kernel.c $(CC_CUST_FLAGS) $(CC_FLAGS) 

loader.o:
	$(ASM_CMD) $(ASM_FLAGS)  -o $(SRC_DIR)/loader.o $(SRC_DIR)/loader.s



clean: 
	@echo "Removing kernel..."
	rm -fv build/kernel.bin
	@echo "Removing object files..."
	rm -fv src/*.o
	@echo "Done! All clean."
