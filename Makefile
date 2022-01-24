CPP = g++
LD = ld
ASM = yasm
ASMFLAGS = -f elf64
CPPFLAGS = -static -m64 -ffreestanding -fno-PIC -fno-PIE -Wall -Wextra -Werror -O2 -fno-exceptions -fno-rtti -nostartfiles -nodefaultlibs -nostdinc++ -mno-red-zone -mno-mmx -mno-sse -mno-sse2
LDFLAGS = -T linker.ld -O2 -nostdlib -m elf_x86_64

KERNEL_PADDING := 512
KERNEL_MIN_BLOCK_COUNT := 1024*1024
ELF_IMAGE = boot.elf
BINARY_IMAGE = boot.img
PROJECT_ROOT := $(shell pwd)
SRC_DIR := $(PROJECT_ROOT)/src
BUILD_DIR := $(PROJECT_ROOT)/out
MACHINE_NAME = Test_VM

INCLUDE_FILES = placement_new.h
INCLUDES = $(INCLUDE_FILES:%=-include $(SRC_DIR)/%)

SRCS := $(shell find $(SRC_DIR) -name '*.c' -or -name '*.asm' -or -name '*.cpp')
SRC_DIRS := $(shell find $(SRC_DIR)/ -type d)
BUILD_DIRS := $(SRC_DIRS:$(SRC_DIR)/%=$(BUILD_DIR)/%)
OBJS := $(SRCS:$(SRC_DIR)/%=$(BUILD_DIR)/%.o)

all: extract_image

link: $(OBJS)
	$(LD) $(LDFLAGS) -o $(BUILD_DIR)/$(ELF_IMAGE) $?

$(BUILD_DIR)/%.c.o: $(SRC_DIR)/%.c
	$(CPP) $(CPPFLAGS) -o $@ -c $<

$(BUILD_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp
	$(CPP) $(CPPFLAGS) $(INCLUDES) -o $@ -c $<

$(BUILD_DIR)/%.asm.o: $(SRC_DIR)/%.asm
	$(ASM) $(ASMFLAGS) $< -o $@

prepare_dirs:
	mkdir -p $(BUILD_DIRS)

extract_image: prepare_dirs link
	./extract_image.sh $(BUILD_DIR) $(ELF_IMAGE) $(BINARY_IMAGE) $(KERNEL_PADDING) $(KERNEL_MIN_BLOCK_COUNT)

clean:
	rm -r out/*

convert: extract_image
	vboxmanage storageattach $(MACHINE_NAME) --storagectl SATA --port 1 --medium none
	vboxmanage closemedium $$(pwd)/$(MACHINE_NAME)/$(MACHINE_NAME).vdi --delete
	vboxmanage convertfromraw $(BUILD_DIR)/$(BINARY_IMAGE) $$(pwd)/$(MACHINE_NAME)/$(MACHINE_NAME).vdi --format VDI
	vboxmanage storageattach $(MACHINE_NAME) --storagectl SATA --port 1 --type hdd --medium $$(pwd)/$(MACHINE_NAME)/$(MACHINE_NAME).vdi

run_vm: convert
	vboxmanage startvm $(MACHINE_NAME)