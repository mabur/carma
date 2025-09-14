#pragma once

#include "make.h"
#include "vector.h"

#if defined(__GNUC__) || defined(__clang__)
    #define TYPE_OF_EXPRESSION(x) typeof(x)
#elif defined(__cplusplus)
    #define TYPE_OF_EXPRESSION(x) decltype(x)
#endif

typedef struct int2x2 {int2 columns[2];} int2x2;
typedef struct int3x3 {int3 columns[3];} int3x3;
typedef struct int4x4 {int4 columns[4];} int4x4;

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

inline float2x2 inverse_float2x2(float2x2 A) {
    float a = A.columns[0][0];
    float b = A.columns[1][0];
    float c = A.columns[0][1];
    float d = A.columns[1][1];

    float det = a * d - b * c;
    float inv_det = 1.f / det;

    float2 result_c0 = (float2){ d, -c } * inv_det;
    float2 result_c1 = (float2){ -b, a } * inv_det;

    return (float2x2){.columns = {result_c0, result_c1}};
}

inline float3x3 inverse_float3x3(float3x3 A) {
    float a = A.columns[0][0], b = A.columns[1][0], c = A.columns[2][0];
    float d = A.columns[0][1], e = A.columns[1][1], f = A.columns[2][1];
    float g = A.columns[0][2], h = A.columns[1][2], i = A.columns[2][2];

    // Cofactors (of A), then we'll transpose to get adj(A)
    float C00 =  e*i - f*h;
    float C01 = -(d*i - f*g);
    float C02 =  d*h - e*g;

    float C10 = -(b*i - c*h);
    float C11 =  a*i - c*g;
    float C12 = -(a*h - b*g);

    float C20 =  b*f - c*e;
    float C21 = -(a*f - c*d);
    float C22 =  a*e - b*d;

    float det = a*C00 + b*C01 + c*C02;
    float inv_det = 1.f / det;

    float3x3 R;
    R.columns[0] = (float3){ C00, C10, C20 } * inv_det;
    R.columns[1] = (float3){ C01, C11, C21 } * inv_det;
    R.columns[2] = (float3){ C02, C12, C22 } * inv_det;
    return R;
}

inline float4x4 inverse_float4x4(float4x4 A) {
    float4 c0 = A.columns[0]; // {a00, a10, a20, a30}
    float4 c1 = A.columns[1]; // {a01, a11, a21, a31}
    float4 c2 = A.columns[2]; // {a02, a12, a22, a32}
    float4 c3 = A.columns[3]; // {a03, a13, a23, a33}

    float a00 = c0[0], a10 = c0[1], a20 = c0[2], a30 = c0[3];
    float a01 = c1[0], a11 = c1[1], a21 = c1[2], a31 = c1[3];
    float a02 = c2[0], a12 = c2[1], a22 = c2[2], a32 = c2[3];
    float a03 = c3[0], a13 = c3[1], a23 = c3[2], a33 = c3[3];

    // Cofactors Cij (unrolled). Column-major output later uses
    // column j = {C0j, C1j, C2j, C3j}.
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

    // Determinant via first row expansion: det = a00*C00 + a01*C10 + a02*C20 + a03*C30
    float det = a00*C00 + a01*C10 + a02*C20 + a03*C30;
    float inv_det = 1.0f / det;

    float4x4 R;
    R.columns[0] = (float4){ C00, C10, C20, C30 } * inv_det;
    R.columns[1] = (float4){ C01, C11, C21, C31 } * inv_det;
    R.columns[2] = (float4){ C02, C12, C22, C32 } * inv_det;
    R.columns[3] = (float4){ C03, C13, C23, C33 } * inv_det;
    return R;
}
