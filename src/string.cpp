#include "string.h"

void memcpy(void *src, void *dst, size_t size) {
    char *dst0 = (char *) dst;
    char *src0 = (char *) src;
    while ((uintptr_t) src0 & 3 && size) {
        *dst0++ = *src0++;
        size--;
    }
    uint32_t *dst1 = (uint32_t *) dst0;
    uint32_t *src1 = (uint32_t *) src0;
    while (size >= 4) {
        *dst1++ = *src1++;
        size -= 4;
    }
    dst0 = (char *) dst1;
    src0 = (char *) src1;
    while (size--) {
        *dst0++ = *src0++;
    }
}

void memset(void *dst, char ch, size_t size) {
    char *dst0 = (char *) dst;
    while ((uintptr_t) dst0 & 3) {
        *dst0++ = ch;
        size--;
    }
    uint32_t l = ch | ch << 8 | ch << 16 | ch << 24;
    uint32_t *dst1 = (uint32_t *) dst0;
    while (size >= 4) {
        *dst1++ = l;
        size -= 4;
    }
    dst0 = (char *) dst1;
    while (size--) {
        *dst0++ = ch;
    }
}

bool memcmp(const void *ptr1, const void *ptr2, size_t size) {
    char *ptr_chr1 = (char *) ptr1;
    char *ptr_chr2 = (char *) ptr2;
    while (size--) {
        if (*ptr_chr1++ != *ptr_chr2++)return false;
    }
    return true;
}

size_t strlen(char *str) {
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}