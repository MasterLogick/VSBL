CPP = c++
LD = ld
ASM = yasm
ASMFLAGS = -f elf32
CPPFLAGS = -m32 --freestanding -fno-pic -Wall -Wextra -Werror -O1 -fno-exceptions -fno-rtti
LDFLAGS = -T linker.ld -O1 -nostdlib -m elf_i386

KERNEL_PADDING := 512
KERNEL_MIN_BLOCK_COUNT := 1024*1024
ELF_IMAGE=boot.elf
BINARY_IMAGE=boot.img
SRC_DIR=./src
BUILD_DIR=./out
MACHINE_NAME=Test_VM

SRCS := $(shell find $(SRC_DIR) -name '*.c' -or -name '*.asm' -or -name '*.c')
SRC_DIRS := $(shell find $(SRC_DIR)/ -type d)
BUILD_DIRS := $(SRC_DIRS:$(SRC_DIR)/%=$(BUILD_DIR)/%)
OBJS := $(SRCS:$(SRC_DIR)/%=$(BUILD_DIR)/%.o)

all: extract_image

link: $(OBJS)
	$(LD) $(LDFLAGS) -o $(BUILD_DIR)/$(ELF_IMAGE) $?

$(BUILD_DIR)/%.c.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CPP) -c $< $(CPPFLAGS) -o $@

$(BUILD_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CPP) -c $< $(CPPFLAGS) -o $@

$(BUILD_DIR)/%.asm.o: $(SRC_DIR)/%.asm
	mkdir -p $(dir $@)
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
	vboxmanage convertfromraw $$(pwd)/$(BUILD_DIR)/$(BINARY_IMAGE) $$(pwd)/$(MACHINE_NAME)/$(MACHINE_NAME).vdi --format VDI
	vboxmanage storageattach $(MACHINE_NAME) --storagectl SATA --port 1 --type hdd --medium $$(pwd)/$(MACHINE_NAME)/$(MACHINE_NAME).vdi

run_vm: convert
	vboxmanage startvm $(MACHINE_NAME)