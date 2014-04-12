ASM_CMD = nasm
ASM_FLAGS = -felf
LD_CMD = ld
GCC_CMD = gcc
CC_CUST_FLAGS = -g
CC_FLAGS = -Wall -Wextra -nostdlib -fno-builtin -nostartfiles -nodefaultlibs

OBJCOPY = objcopy
OBJCOPY_FLAGS = --only-keep-debug

BUILD_DIR = build
SRC_DIR = src
SCRIPT_DIR = scripts
LINKER_DIR = src
LINKER_SCRIPT = linker.ld
KERNEL_IMAGE = kernel.bin
KERNEL_SYMBOLS = kernel.sym
.PHONY: all

all: clean kernel symbols strip

strip:
	$(OBJCOPY) --strip-debug $(BUILD_DIR)/$(KERNEL_IMAGE)

symbols:
	$(OBJCOPY) $(OBJCOPY_FLAGS) $(BUILD_DIR)/$(KERNEL_IMAGE) $(BUILD_DIR)/$(KERNEL_SYMBOLS)

kernel: kernel.o loader.o console.o
	$(LD_CMD) -T  $(LINKER_DIR)/$(LINKER_SCRIPT) -o $(BUILD_DIR)/$(KERNEL_IMAGE) $(SRC_DIR)/loader.o $(SRC_DIR)/kernel.o $(SRC_DIR)/console.o

kernel.o:
	$(GCC_CMD) -o $(SRC_DIR)/kernel.o -c $(SRC_DIR)/kernel.c $(CC_CUST_FLAGS) $(CC_FLAGS) 

loader.o:
	$(ASM_CMD) $(ASM_FLAGS)  -o $(SRC_DIR)/loader.o $(SRC_DIR)/loader.s

console.o:
	$(GCC_CMD) -o $(SRC_DIR)/console.o -c $(SRC_DIR)/console.c $(CC_CUST_FLAGS) $(CC_FLAGS) 

clean: 
	@echo "Removing kernel..."
	rm -fv build/kernel.bin
	@echo "Removing object files..."
	rm -fv src/*.o
	@echo "Done! All clean."
