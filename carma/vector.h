#pragma once

#include <math.h>

#if defined(__clang__) || defined(__GNUC__)

typedef int Vec2i __attribute__((vector_size(8)));
typedef int Vec3i __attribute__((vector_size(16))); // Including padding
typedef int Vec4i __attribute__((vector_size(16)));

typedef float Vec2f __attribute__((vector_size(8)));
typedef float Vec3f __attribute__((vector_size(16))); // Including padding
typedef float Vec4f __attribute__((vector_size(16)));

typedef double Vec2d __attribute__((vector_size(16)));
typedef double Vec3d __attribute__((vector_size(32))); // Including padding
typedef double Vec4d __attribute__((vector_size(32)));

#elif defined(__cplusplus)

template<typename T, size_t D>
struct VecTemp {
    T data[D];
    // Indexing:
    T& operator[](size_t i) { return data[i]; }
    const T& operator[](size_t i) const { return data[i]; }
    // Elementwise vector assignment operators
    VecTemp& operator+=(const VecTemp& b) {
        for (size_t i = 0; i < D; ++i) data[i] += b[i];
        return *this;
    }
    VecTemp& operator-=(const VecTemp& b) {
        for (size_t i = 0; i < D; ++i) data[i] -= b[i];
        return *this;
    }
    VecTemp& operator*=(const VecTemp& b) {
        for (size_t i = 0; i < D; ++i) data[i] *= b[i];
        return *this;
    }
    VecTemp& operator/=(const VecTemp& b) {
        for (size_t i = 0; i < D; ++i) data[i] /= b[i];
        return *this;
    }
    VecTemp& operator*=(T scalar) {
        for (size_t i = 0; i < D; ++i) data[i] *= scalar;
        return *this;
    }
    VecTemp& operator/=(T scalar) {
        for (size_t i = 0; i < D; ++i) data[i] /= scalar;
        return *this;
    }
    // Vector-vector arithmetics
    friend VecTemp operator+(VecTemp a, const VecTemp& b) {return a += b;}
    friend VecTemp operator-(VecTemp a, const VecTemp& b) {return a -= b;}
    friend VecTemp operator*(VecTemp a, const VecTemp& b) {return a *= b;}
    friend VecTemp operator/(VecTemp a, const VecTemp& b) {return a /= b;}
    // Vector-scalar arithmetics
    friend VecTemp operator*(VecTemp a, T scalar) {return a *= scalar;}
    friend VecTemp operator*(T scalar, VecTemp b) {return b *= scalar;}
    friend VecTemp operator/(VecTemp a, T scalar) {return a /= scalar;}
    // Unary minus
    friend VecTemp operator-(const VecTemp& v) {return VecTemp{} -= v;}
};

typedef VecTemp<int, 2> Vec2i;
typedef VecTemp<int, 3> Vec3i;
typedef VecTemp<int, 4> Vec4i;

typedef VecTemp<float, 2> Vec2f;
typedef VecTemp<float, 3> Vec3f;
typedef VecTemp<float, 4> Vec4f;

typedef VecTemp<double, 2> Vec2d;
typedef VecTemp<double, 3> Vec3d;
typedef VecTemp<double, 4> Vec4d;

#else
#error "The vector types require either clang or gcc or C++"
#endif

#define DOT2(a, b) ((a)[0] * (b)[0] + (a)[1] * (b)[1])
#define DOT3(a, b) ((a)[0] * (b)[0] + (a)[1] * (b)[1] + (a)[2] * (b)[2])

#define SQUARED_NORM2(a) DOT2((a), (a))
#define SQUARED_NORM3(a) DOT3((a), (a))

#define NORM2(a) (sqrt(SQUARED_NORM2(a)))
#define NORM3(a) (sqrt(SQUARED_NORM3(a)))

#define SQUARED_DISTANCE2(a, b) SQUARED_NORM2((a) - (b))
#define SQUARED_DISTANCE3(a, b) SQUARED_NORM3((a) - (b))

#define DISTANCE2(a, b) NORM2((a) - (b))
#define DISTANCE3(a, b) NORM3((a) - (b))
