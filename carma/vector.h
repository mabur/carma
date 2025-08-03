#pragma once

#include <math.h>

#if defined(__clang__) || defined(__GNUC__)

typedef int int2 __attribute__((vector_size(8)));
typedef int int3 __attribute__((vector_size(16))); // Including padding
typedef int int4 __attribute__((vector_size(16)));

typedef float float2 __attribute__((vector_size(8)));
typedef float float3 __attribute__((vector_size(16))); // Including padding
typedef float float4 __attribute__((vector_size(16)));

typedef double double2 __attribute__((vector_size(16)));
typedef double double3 __attribute__((vector_size(32))); // Including padding
typedef double double4 __attribute__((vector_size(32)));

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

typedef VecTemp<int, 2> int2;
typedef VecTemp<int, 3> int3;
typedef VecTemp<int, 4> int4;

typedef VecTemp<float, 2> float2;
typedef VecTemp<float, 3> float3;
typedef VecTemp<float, 4> float4;

typedef VecTemp<double, 2> double2;
typedef VecTemp<double, 3> double3;
typedef VecTemp<double, 4> double4;

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
