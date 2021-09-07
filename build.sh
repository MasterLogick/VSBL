#!/bin/bash
yasm -f elf32 src/boot.asm -o out/boot.o
cc -c src/kernel.c -o out/kernel.o -m32 --freestanding -fstrength-reduce -fomit-frame-pointer -finline-functions -fno-builtin -nostdlib -nostartfiles -nodefaultlibs -Wall -Werror -Iinc -O2
ld -T linker.ld -o out/boot.elf -O2 -nostdlib -m elf_i386 out/boot.o out/kernel.o
objcopy -O binary out/boot.elf out/boot.img
#padd boot.img to x512 bytes size
REAL_SIZE=$(stat -c%s out/boot.img)
mv out/boot.img out/boot.img.old
PADDING=512
BLOCK_COUNT=$((($REAL_SIZE+$PADDING-1)/$PADDING))
BLOCK_COUNT=$(($BLOCK_COUNT>1024*1024/$PADDING ? $BLOCK_COUNT : 1024*1024/$PADDING))
dd if=/dev/zero of=out/boot.img bs=$PADDING count=$BLOCK_COUNT
dd if=out/boot.img.old of=out/boot.img bs=1 count=$REAL_SIZE seek=0 conv=notrunc

BASEDIR=$(pwd)
vboxmanage storageattach Test_VM_64 --storagectl SATA --port 1 --medium none
vboxmanage closemedium $BASEDIR/Test_VM_64/Test_VM_64.vdi --delete
vboxmanage convertfromraw $BASEDIR/out/boot.img $BASEDIR/Test_VM_64/Test_VM_64.vdi --format VDI
vboxmanage storageattach Test_VM_64 --storagectl SATA --port 1 --type hdd --medium $BASEDIR/Test_VM_64/Test_VM_64.vdi
