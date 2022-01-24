#ifndef STRING_C
#define STRING_C

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

void memcpy(void *src, void *dst, size_t size);

void memset(void *dst, char ch, size_t size);

bool memcmp(const void *ptr1, const void *ptr2, size_t size);

size_t strlen(const char *str);

#endif //STRING_C