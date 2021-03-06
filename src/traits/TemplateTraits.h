#ifndef TEMPLATETRAITS_H
#define TEMPLATETRAITS_H

template<typename, typename>
struct ReplaceFirstArgument {
};

template<template<typename, typename ...> class Outer, typename Replacement, typename Replacer, typename ...Args>
struct ReplaceFirstArgument<Outer<Replacement, Args...>, Replacer> {
    typedef Outer<Replacer, Args...> Type;
};

template<typename ...>
using void_t = void;

#endif //TEMPLATETRAITS_H
