#ifndef NUMBERTRAITS_H
#define NUMBERTRAITS_H

#include <stdint.h>

template<typename T>
struct NumberTraits;

template<>
struct NumberTraits<uint8_t> {
    static const uint8_t min = 0;
    static const uint8_t max = UINT8_MAX;
    static const uint8_t width = 8;
    static const bool sign = false;
};

template<>
struct NumberTraits<int8_t> {
    static const int8_t min = INT8_MIN;
    static const int8_t max = INT8_MAX;
    static const uint8_t width = 8;
    static const bool sign = true;
};

template<>
struct NumberTraits<uint16_t> {
    static const uint16_t min = 0;
    static const uint16_t max = UINT16_MAX;
    static const uint8_t width = 16;
    static const bool sign = false;
};

template<>
struct NumberTraits<int16_t> {
    static const int16_t min = INT16_MIN;
    static const int16_t max = INT16_MAX;
    static const uint8_t width = 16;
    static const bool sign = true;
};

template<>
struct NumberTraits<uint32_t> {
    static const uint32_t min = 0;
    static const uint32_t max = UINT32_MAX;
    static const uint8_t width = 32;
    static const bool sign = false;
};
template<>
struct NumberTraits<int32_t> {
    static const int32_t min = INT32_MIN;
    static const int32_t max = INT32_MAX;
    static const uint8_t width = 32;
    static const bool sign = true;
};
template<>
struct NumberTraits<uint64_t> {
    static const uint64_t min = 0;
    static const uint64_t max = UINT64_MAX;
    static const uint8_t width = 64;
    static const bool sign = false;
};
template<>
struct NumberTraits<int64_t> {
    static const int64_t min = INT64_MIN;
    static const int64_t max = INT64_MAX;
    static const uint8_t width = 64;
    static const bool sign = true;
};

#endif //NUMBERTRAITS_H
