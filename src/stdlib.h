#ifndef STDLIB_H
#define STDLIB_H

template<typename T>
void swap(T *a, T *b) {
    T tmp = *a;
    *a = *b;
    *b = tmp;
}

template<typename T>
T *partition(T *left, T *right, int (*comp)(T *, T *)) {
    T *pivot = right;
    T *i = left - 1;
    for (T *j = left; j <= right - 1; ++j) {
        if (comp(j, pivot) <= 0) {
            i++;
            swap(i, j);
        }
    }
    swap(i + 1, right);
    return i + 1;
}

template<typename T>
void qs(T *left, T *right, int (*comp)(T *, T *)) {
    if (left >= right) {
        return;
    }
    T *part = partition(left, right, comp);
    qs(left, part - 1, comp);
    qs(part + 1, right, comp);
}

template<typename T>
void qsort(T *base, size_t items, int (*comp)(T *, T *)) {
    qs(base, base + items - 1, comp);
}

#endif //STDLIB_H
