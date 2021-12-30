objcopy -O binary "$1"/"$2" "$1"/"$3".old
REAL_SIZE=$(stat -c%s "$1"/"$3".old)
BLOCK_COUNT=$(((REAL_SIZE + $4 - 1) / $4))
BLOCK_COUNT=$((BLOCK_COUNT > $(($5)) ? BLOCK_COUNT : $(($5)) / $4))
dd if=/dev/zero of="$1"/"$3" bs="$4" count=$BLOCK_COUNT
dd if="$1"/"$3".old of="$1"/"$3" bs=1 count="$REAL_SIZE" seek=0 conv=notrunc
