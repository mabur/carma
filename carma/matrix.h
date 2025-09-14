#pragma once

#include "make.h"
#include "vector.h"

#if defined(__GNUC__) || defined(__clang__)
    #define TYPE_OF_EXPRESSION(x) typeof(x)
#elif defined(__cplusplus)
    #define TYPE_OF_EXPRESSION(x) decltype(x)
#endif

typedef struct float2x2 {float2 columns[2];} float2x2;
typedef struct float3x3 {float3 columns[3];} float3x3;
typedef struct float4x4 {float4 columns[4];} float4x4;

typedef struct double2x2 {double2 columns[2];} double2x2;
typedef struct double3x3 {double3 columns[3];} double3x3;
typedef struct double4x4 {double4 columns[4];} double4x4;

#define MUL_2x2_2(A, b) ( \
    (A).columns[0] * (b)[0] + \
    (A).columns[1] * (b)[1]   \
)

#define MUL_3x3_3(A, b) ( \
    (A).columns[0] * (b)[0] + \
    (A).columns[1] * (b)[1] + \
    (A).columns[2] * (b)[2]   \
)

#define MUL_4x4_4(A, b) ( \
    (A).columns[0] * (b)[0] + \
    (A).columns[1] * (b)[1] + \
    (A).columns[2] * (b)[2] + \
    (A).columns[3] * (b)[3]   \
)

inline float2 mul_float2x2_float2(float2x2 A, float2 b) {
    return MUL_2x2_2(A, b);
}

inline float3 mul_float3x3_float3(float3x3 A, float3 b) {
    return MUL_3x3_3(A, b);
}

inline float4 mul_float4x4_float4(float4x4 A, const float4 b) {
    return MUL_4x4_4(A, b);
}

#define SET_INVERSE_2x2(in, out) do { \
    auto a = (in).columns[0][0]; \
    auto b = (in).columns[1][0]; \
    auto c = (in).columns[0][1]; \
    auto d = (in).columns[1][1]; \
    auto det = a * d - b * c; \
    auto inv_det = 1 / det; \
    (out).columns[0][0] = d; \
    (out).columns[0][1] = -c; \
    (out).columns[1][0] = -b; \
    (out).columns[1][1] = a; \
    (out).columns[0] *= inv_det; \
    (out).columns[1] *= inv_det; \
} while (0)

inline float2x2 inverse_float2x2(float2x2 A) {
    float2x2 result;
    SET_INVERSE_2x2(A, result);
    return result;
}

inline double2x2 inverse_double2x2(double2x2 A) {
    double2x2 result;
    SET_INVERSE_2x2(A, result);
    return result;
}

#define SET_INVERSE_3x3(in, out) do { \
    auto a00 = (in).columns[0][0]; \
    auto a01 = (in).columns[1][0]; \
    auto a02 = (in).columns[2][0]; \
    auto a10 = (in).columns[0][1]; \
    auto a11 = (in).columns[1][1]; \
    auto a12 = (in).columns[2][1]; \
    auto a20 = (in).columns[0][2]; \
    auto a21 = (in).columns[1][2]; \
    auto a22 = (in).columns[2][2]; \
    \
    auto C00 =  (a11 * a22 - a12 * a21); \
    auto C01 = -(a10 * a22 - a12 * a20); \
    auto C02 =  (a10 * a21 - a11 * a20); \
    \
    auto C10 = -(a01 * a22 - a02 * a21); \
    auto C11 =  (a00 * a22 - a02 * a20); \
    auto C12 = -(a00 * a21 - a01 * a20); \
    \
    auto C20 =  (a01 * a12 - a02 * a11); \
    auto C21 = -(a00 * a12 - a02 * a10); \
    auto C22 =  (a00 * a11 - a01 * a10); \
    \
    auto det = a00 * C00 + a01 * C01 + a02 * C02; \
    auto inv_det = (1) / det; \
    \
    (out).columns[0][0] = C00; (out).columns[1][0] = C01; (out).columns[2][0] = C02; \
    (out).columns[0][1] = C10; (out).columns[1][1] = C11; (out).columns[2][1] = C12; \
    (out).columns[0][2] = C20; (out).columns[1][2] = C21; (out).columns[2][2] = C22; \
    \
    (out).columns[0] *= inv_det; \
    (out).columns[1] *= inv_det; \
    (out).columns[2] *= inv_det; \
} while (0)


inline float3x3 inverse_float3x3(float3x3 A) {
    float3x3 result;
    SET_INVERSE_3x3(A, result);
    return result;
}

inline double3x3 inverse_double3x3(double3x3 A) {
    double3x3 result;
    SET_INVERSE_3x3(A, result);
    return result;
}

inline float4x4 inverse_float4x4(float4x4 A) {
    float a00 = A.columns[0][0], a01 = A.columns[1][0], a02 = A.columns[2][0], a03 = A.columns[3][0];
    float a10 = A.columns[0][1], a11 = A.columns[1][1], a12 = A.columns[2][1], a13 = A.columns[3][1];
    float a20 = A.columns[0][2], a21 = A.columns[1][2], a22 = A.columns[2][2], a23 = A.columns[3][2];
    float a30 = A.columns[0][3], a31 = A.columns[1][3], a32 = A.columns[2][3], a33 = A.columns[3][3];

    float C00 =  a11*a22*a33 - a11*a23*a32 - a21*a12*a33 + a21*a13*a32 + a31*a12*a23 - a31*a13*a22;
    float C01 = -a01*a22*a33 + a01*a23*a32 + a21*a02*a33 - a21*a03*a32 - a31*a02*a23 + a31*a03*a22;
    float C02 =  a01*a12*a33 - a01*a13*a32 - a11*a02*a33 + a11*a03*a32 + a31*a02*a13 - a31*a03*a12;
    float C03 = -a01*a12*a23 + a01*a13*a22 + a11*a02*a23 - a11*a03*a22 - a21*a02*a13 + a21*a03*a12;

    float C10 = -a10*a22*a33 + a10*a23*a32 + a20*a12*a33 - a20*a13*a32 - a30*a12*a23 + a30*a13*a22;
    float C11 =  a00*a22*a33 - a00*a23*a32 - a20*a02*a33 + a20*a03*a32 + a30*a02*a23 - a30*a03*a22;
    float C12 = -a00*a12*a33 + a00*a13*a32 + a10*a02*a33 - a10*a03*a32 - a30*a02*a13 + a30*a03*a12;
    float C13 =  a00*a12*a23 - a00*a13*a22 - a10*a02*a23 + a10*a03*a22 + a20*a02*a13 - a20*a03*a12;

    float C20 =  a10*a21*a33 - a10*a23*a31 - a20*a11*a33 + a20*a13*a31 + a30*a11*a23 - a30*a13*a21;
    float C21 = -a00*a21*a33 + a00*a23*a31 + a20*a01*a33 - a20*a03*a31 - a30*a01*a23 + a30*a03*a21;
    float C22 =  a00*a11*a33 - a00*a13*a31 - a10*a01*a33 + a10*a03*a31 + a30*a01*a13 - a30*a03*a11;
    float C23 = -a00*a11*a23 + a00*a13*a21 + a10*a01*a23 - a10*a03*a21 - a20*a01*a13 + a20*a03*a11;

    float C30 = -a10*a21*a32 + a10*a22*a31 + a20*a11*a32 - a20*a12*a31 - a30*a11*a22 + a30*a12*a21;
    float C31 =  a00*a21*a32 - a00*a22*a31 - a20*a01*a32 + a20*a02*a31 + a30*a01*a22 - a30*a02*a21;
    float C32 = -a00*a11*a32 + a00*a12*a31 + a10*a01*a32 - a10*a02*a31 - a30*a01*a12 + a30*a02*a11;
    float C33 =  a00*a11*a22 - a00*a12*a21 - a10*a01*a22 + a10*a02*a21 + a20*a01*a12 - a20*a02*a11;

    float det = a00*C00 + a01*C10 + a02*C20 + a03*C30;
    float inv_det = 1.0f / det;

    float4x4 R;
    R.columns[0] = (float4){ C00, C10, C20, C30 } * inv_det;
    R.columns[1] = (float4){ C01, C11, C21, C31 } * inv_det;
    R.columns[2] = (float4){ C02, C12, C22, C32 } * inv_det;
    R.columns[3] = (float4){ C03, C13, C23, C33 } * inv_det;
    return R;
}
