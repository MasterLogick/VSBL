#ifndef STRING_C
#define STRING_C

#include <stdint.h>
#include <stddef.h>

void memcpy(void *src, void *dst, size_t size);

void memset(void *dst, char ch, size_t size);

size_t strlen(char *str);

#endif //STRING_C