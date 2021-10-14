CC = cc
LD = ld
ASM = yasm
ASMFLAGS = -f elf32
CFLAGS = -m32 --freestanding -fno-pic -Wall -Wextra -Werror -O1 -std=gnu99
LDFLAGS = -T linker.ld -O1 -nostdlib -m elf_i386

KERNEL_PADDING := 512
KERNEL_MIN_BLOCK_COUNT := 1024*1024
ELF_IMAGE=boot.elf
BINARY_IMAGE=boot.img
SRC_DIR=./src
BUILD_DIR=./out
MACHINE_NAME=Test_VM

SRCS := $(shell find $(SRC_DIR) -name '*.c' -or -name '*.asm')
SRC_DIRS := $(shell find $(SRC_DIR)/ -type d)
BUILD_DIRS := $(SRC_DIRS:$(SRC_DIR)/%=$(BUILD_DIR)/%)
OBJS := $(SRCS:$(SRC_DIR)/%=$(BUILD_DIR)/%.o)

all: extract_image

link: $(OBJS)
	$(LD) $(LDFLAGS) -o $(BUILD_DIR)/$(ELF_IMAGE) $?

$(BUILD_DIR)/%.c.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) -c $< $(CFLAGS) -o $@

$(BUILD_DIR)/%.asm.o: $(SRC_DIR)/%.asm
	mkdir -p $(dir $@)
	$(ASM) $(ASMFLAGS) $< -o $@

prepare_dirs: $(BUILD_DIRS)
	mkdir -p $?

extract_image: prepare_dirs link
	objcopy -O binary $(BUILD_DIR)/$(ELF_IMAGE) $(BUILD_DIR)/$(BINARY_IMAGE).old
	export REAL_SIZE=$$(stat -c%s $(BUILD_DIR)/$(BINARY_IMAGE).old);\
	export BLOCK_COUNT=$$((($$REAL_SIZE+$(KERNEL_PADDING)-1)/$(KERNEL_PADDING)));\
	export BLOCK_COUNT=$$(($$BLOCK_COUNT>$$(($(KERNEL_MIN_BLOCK_COUNT))) ? $$BLOCK_COUNT : $$(($(KERNEL_MIN_BLOCK_COUNT)))/$(KERNEL_PADDING)));\
	dd if=/dev/zero of=$(BUILD_DIR)/$(BINARY_IMAGE) bs=$(KERNEL_PADDING) count=$$BLOCK_COUNT;\
    dd if=$(BUILD_DIR)/$(BINARY_IMAGE).old of=$(BUILD_DIR)/$(BINARY_IMAGE) bs=1 count=$$REAL_SIZE seek=0 conv=notrunc;

clean:
	rm -r out/*

convert: extract_image
	vboxmanage storageattach $(MACHINE_NAME) --storagectl SATA --port 1 --medium none
	vboxmanage closemedium $$(pwd)/$(MACHINE_NAME)/$(MACHINE_NAME).vdi --delete
	vboxmanage convertfromraw $$(pwd)/$(BUILD_DIR)/$(BINARY_IMAGE) $$(pwd)/$(MACHINE_NAME)/$(MACHINE_NAME).vdi --format VDI
	vboxmanage storageattach $(MACHINE_NAME) --storagectl SATA --port 1 --type hdd --medium $$(pwd)/$(MACHINE_NAME)/$(MACHINE_NAME).vdi

run_vm: extract_image
	vboxmanage startvm $(MACHINE_NAME)