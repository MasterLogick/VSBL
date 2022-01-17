#ifndef MATH_H
#define MATH_H

template<typename T>
constexpr T min(T a, T b) {
    return (a < b) ? a : b;
}

template<typename T>
constexpr T max(T a, T b) {
    return (a > b) ? a : b;
}

#endif //MATH_H
