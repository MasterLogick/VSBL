#ifndef ALLOC_TRAITS_H
#define ALLOC_TRAITS_H

#include "TemplateTraits.h"

template<typename Alloc>
struct AllocatorTraits {
    typedef typename Alloc::ValueType ValueType;
    typedef typename Alloc::SizeType SizeType;
    typedef Alloc BaseType;

    template<typename NewT, typename = void>
    struct Rebind {
        typedef AllocatorTraits<typename ReplaceFirstArgument<Alloc, NewT>::Type> Other;
    };

    template<typename NewT>
    struct Rebind<NewT, void_t<typename Alloc::template Rebind<NewT>::Other>> {
        typedef AllocatorTraits<typename Alloc::template Rebind<NewT>::Other> Other;
    };

    static inline ValueType *allocate(Alloc &a, SizeType n) {
        return a.allocate(n);
    }

    static inline void allocate(Alloc &a, ValueType *ptr) {
        return a.deallocate(ptr);
    }
};

#endif //ALLOC_TRAITS_H
