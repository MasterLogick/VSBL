#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include "algorithm.h"
#include "math.h"

template<typename V, typename Alloc = Allocator<>>
class Vector {
    V *array;
    size_t size;
    size_t capacity;
    typename AllocatorTraits<Alloc>::template Rebind<V>::Other::BaseType allocator;

    template<typename T>
    void push_backT(T val) {
        if (capacity <= size) {
            resize(size * 2);
        }
        array[size] = val;
    }

public:
    Vector() : Vector(1) {};

    explicit Vector(size_t size) : size(0), capacity(size) {
        array = new(allocator.allocate(capacity))V[capacity];
    }

    inline size_t getSize() {
        return size;
    }

    inline void push_back(const V &val) {
        push_backT(val);
    }

    inline void push_back(V &&val) {
        push_backT(val);
    }

    inline V pop_back() {
        remove(size - 1);
        return this->operator[](getSize());
    }

    void remove(size_t i) {
        if (i < size) {
            copy(array + i, array + i + 1, size - i - 1);
            size--;
        }
    }

    V &operator[](size_t i) {
        return array[i];
    }

    void resize(size_t n) {
        if (capacity < n) {
            V *tmpArray = new(allocator.allocate(n))V[n];
            copy(tmpArray, array, capacity);
            allocator.deallocate(array);
            array = tmpArray;
            capacity = n;
        }
    }

};

#endif //VECTOR_H
