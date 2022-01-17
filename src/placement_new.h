#ifndef NEW_H
#define NEW_H

#include <stddef.h>

inline void *operator new(size_t, void *__p) { return __p; }

inline void *operator new[](size_t, void *__p) { return __p; }

inline void operator delete(void *, void *) {}

inline void operator delete[](void *, void *) {}

#endif //NEW_H
