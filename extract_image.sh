objcopy -O binary "$1" "$2".old
REAL_SIZE=$(stat -c%s "$2".old)
BLOCK_COUNT=$(((REAL_SIZE + $3 - 1) / $3))
BLOCK_COUNT=$((BLOCK_COUNT > $((1024*1024)) ? BLOCK_COUNT : $((1024*1024)) / $3))
dd if=/dev/zero of="$2" bs="$3" count=$BLOCK_COUNT
dd if="$2".old of="$2" bs=1 count="$REAL_SIZE" seek=0 conv=notrunc
rm "$2".old