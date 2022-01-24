#include "string.h"

void memcpy(void *src, void *dst, size_t size) {
    uint64_t *dst1 = (uint64_t *) dst;
    uint64_t *src1 = (uint64_t *) src;
    while (size >= 8) {
        *dst1++ = *src1++;
        size -= 8;
    }
    uint8_t *dst0 = (uint8_t *) dst1;
    uint8_t *src0 = (uint8_t *) src1;
    while (size--) {
        *dst0++ = *src0++;
    }
}

void memset(void *dst, char ch, size_t size) {
    uint8_t *dst0 = (uint8_t *) dst;
    while ((uintptr_t) dst0 & 7) {
        *dst0++ = ch;
        size--;
    }
    uint64_t l = ((uint64_t) ch) | ((uint64_t) ch << 8) | ((uint64_t) ch << 16) | ((uint64_t) ch << 24) |
                 ((uint64_t) ch << 32) | ((uint64_t) ch << 40) | ((uint64_t) ch << 48) | ((uint64_t) ch << 56);
    uint64_t *dst1 = (uint64_t *) dst0;
    while (size >= 8) {
        *dst1++ = l;
        size -= 8;
    }
    dst0 = (uint8_t *) dst1;
    while (size--) {
        *dst0++ = ch;
    }
}

bool memcmp(const void *ptr1, const void *ptr2, size_t size) {
    uint8_t *ptr_chr1 = (uint8_t *) ptr1;
    uint8_t *ptr_chr2 = (uint8_t *) ptr2;
    while (size--) {
        if (*ptr_chr1++ != *ptr_chr2++)return false;
    }
    return true;
}

size_t strlen(const char *str) {
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}