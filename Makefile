ASM_CMD = nasm
ASM_FLAGS = -felf
LD_CMD = ld
GCC_CMD = gcc
CC_CUST_FLAGS = -g
CC_FLAGS = -std=gnu99 -Wall -Wextra -nostdlib -fno-builtin -nostartfiles -nodefaultlibs

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

kernel: kernel.o loader.o console.o string.o idt.o gdt.o
	$(LD_CMD) -T  $(LINKER_DIR)/$(LINKER_SCRIPT) -o $(BUILD_DIR)/$(KERNEL_IMAGE) $(SRC_DIR)/gdt.o $(SRC_DIR)/string.o $(SRC_DIR)/idt.o $(SRC_DIR)/loader.o $(SRC_DIR)/kernel.o $(SRC_DIR)/console.o $(SRC_DIR)/gdtasm.o

idtasm.o:
	$(ASM_CMD) $(ASM_FLAGS)  -o $(SRC_DIR)/idtasm.o $(SRC_DIR)/idtasm.s

idt.o:
	$(GCC_CMD) -o $(SRC_DIR)/idt.o -c $(SRC_DIR)/idt.c $(CC_CUST_FLAGS) $(CC_FLAGS) 

string.o:
	$(GCC_CMD) -o $(SRC_DIR)/string.o -c $(SRC_DIR)/string.c $(CC_CUST_FLAGS) $(CC_FLAGS) 
    
kernel.o:
	$(GCC_CMD) -o $(SRC_DIR)/kernel.o -c $(SRC_DIR)/kernel.c $(CC_CUST_FLAGS) $(CC_FLAGS) 

loader.o:
	$(ASM_CMD) $(ASM_FLAGS)  -o $(SRC_DIR)/loader.o $(SRC_DIR)/loader.s

console.o:
	$(GCC_CMD) -o $(SRC_DIR)/console.o -c $(SRC_DIR)/console.c $(CC_CUST_FLAGS) $(CC_FLAGS) 

gdt.o:
	$(GCC_CMD) -o $(SRC_DIR)/gdt.o -c $(SRC_DIR)/gdt.c $(CC_CUST_FLAGS) $(CC_FLAGS) 
	$(ASM_CMD) $(ASM_FLAGS) -o $(SRC_DIR)/gdtasm.o $(SRC_DIR)/gdt.s

clean: 
	@echo "Removing kernel..."
	rm -fv build/kernel.bin
	@echo "Removing object files..."
	rm -fv src/*.o
	@echo "Done! All clean."
