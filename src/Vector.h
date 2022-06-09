#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm.h>
#include <math.h>
#include <Allocator.h>
#include <traits/AllocatorTraits.h>

template<typename V, typename Alloc = Allocator<>>
class Vector {
    V *array;
    int size;
    int capacity;
    typename AllocatorTraits<Alloc>::template Rebind<V>::Other::BaseType allocator;

    template<typename T>
    void push_backT(T val) {
        if (capacity <= size) {
            resize(size * 2);
        }
        array[size] = val;
        size++;
    }

public:
    Vector() : Vector(1) {};

    explicit Vector(int size) : size(0), capacity(size) {
        array = reinterpret_cast<V *>(allocator.allocate(capacity));
    }

    inline int getSize() {
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

    void remove(int i) {
        if (i < size) {
            copy(array + i, array + i + 1, size - i - 1);
            size--;
        }
    }

    V &operator[](size_t i) {
        return array[i];
    }

    void resize(int n) {
        if (capacity < n) {
            V *tmpArray = reinterpret_cast<V *>(allocator.allocate(n));
            copy(tmpArray, array, capacity);
            allocator.deallocate(array);
            array = tmpArray;
            capacity = n;
        }
    }

    inline V *begin() {
        return array;
    }

    inline V *end() {
        return array + size;
    }

};

#endif //VECTOR_H
