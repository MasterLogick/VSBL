#!/bin/bash
IMAGE_NAME="boot.img"
yasm -f elf32 src/boot.asm -o out/boot.o
yasm -f elf32 src/secondary.asm -o out/secondary.o
cc -c src/kernel.c -o out/kernel.o -m32 --freestanding -fno-pic -Wall -Wextra -Werror -O2
ld -T linker.ld -o out/boot.elf -O2 -nostdlib -m elf_i386 out/boot.o out/secondary.o out/kernel.o
objcopy -O binary out/boot.elf out/$IMAGE_NAME
#padd boot.img to x512 bytes size
REAL_SIZE=$(stat -c%s out/$IMAGE_NAME)
mv out/$IMAGE_NAME out/$IMAGE_NAME.old
PADDING=512
BLOCK_COUNT=$((($REAL_SIZE+$PADDING-1)/$PADDING))
BLOCK_COUNT=$(($BLOCK_COUNT>1024*1024/$PADDING ? $BLOCK_COUNT : 1024*1024/$PADDING))
dd if=/dev/zero of=out/$IMAGE_NAME bs=$PADDING count=$BLOCK_COUNT
dd if=out/$IMAGE_NAME.old of=out/$IMAGE_NAME bs=1 count=$REAL_SIZE seek=0 conv=notrunc