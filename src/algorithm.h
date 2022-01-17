#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <stddef.h>

template<typename T>
void copy(T *dst, T *src, size_t size) {
    while (size--) {
        *dst++ = *src++;
    }
}

#endif //ALGORITHM_H
