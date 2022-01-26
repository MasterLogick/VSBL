#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

template<typename T = void>
struct Allocator {
    typedef T ValueType;
    typedef size_t SizeType;

    template<typename U>
    struct Rebind {
        typedef Allocator<U> Other;
    };

    inline T *allocate(SizeType n) {
        return reinterpret_cast<T *>(operator new(n * sizeof(T)));
    }

    inline void deallocate(T *ptr) {
        delete ptr;
    }
};

#endif //ALLOCATOR_H
