#!/bin/bash
yasm -f elf32 src/boot.asm -o out/boot.o
cc --freestanding -O2 -c src/kernel.c -o out/kernel.o -m32
ld -T linker.ld -o out/boot.bin -O2 --nostdlib out/boot.o out/kernel.o -m elf_i386
objcopy -O binary out/boot.bin out/boot.img
#padd boot.img to x512 bytes size
REAL_SIZE=$(stat -c%s out/boot.img)
mv out/boot.img out/boot.img.old
PADDING=512
BLOCK_COUNT=$((($REAL_SIZE+$PADDING-1)/$PADDING))
BLOCK_COUNT=$(($BLOCK_COUNT>1024*1024/$PADDING ? $BLOCK_COUNT : 1024*1024/$PADDING))
dd if=/dev/zero of=out/boot.img bs=$PADDING count=$BLOCK_COUNT
dd if=out/boot.img.old of=out/boot.img bs=1 count=$REAL_SIZE seek=0 conv=notrunc