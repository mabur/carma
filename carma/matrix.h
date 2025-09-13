#pragma once

#include "vector.h"

typedef struct {
    float2 columns[2];
} float2x2;

typedef struct {
    float3 columns[3]; // column-major: columns[0], columns[1], columns[2]
} float3x3;

typedef struct {
    float4 columns[4];
} float4x4;

inline float2 mul_float2x2_float2(float2x2 A, float2 v) {
    return
        A.columns[0] * v[0] +
        A.columns[1] * v[1];
}

inline float3 mul_float3x3_float3(float3x3 A, const float3 v) {
    return
        A.columns[0] * v[0] +
        A.columns[1] * v[1] +
        A.columns[2] * v[2];
}

inline float4 mul_float4x4_float4(float4x4 A, const float4 b) {
    return
        A.columns[0] * b[0] +
        A.columns[1] * b[1] +
        A.columns[2] * b[2] +
        A.columns[3] * b[3];
}

inline float2x2 inverse_float2x2(float2x2 A) {
    float2 c0 = A.columns[0];
    float2 c1 = A.columns[1];

    float a = c0[0];
    float b = c1[0];
    float c = c0[1];
    float d = c1[1];

    float det = a * d - b * c;
    float inv_det = 1.f / det;

    float2 result_c0 = (float2){ d, -c } * inv_det;
    float2 result_c1 = (float2){ -b, a } * inv_det;

    return (float2x2){.columns = {result_c0, result_c1}};
}

inline float3x3 inverse_float3x3(float3x3 A) {
    float3 c0 = A.columns[0];
    float3 c1 = A.columns[1];
    float3 c2 = A.columns[2];

    float a = c0[0], b = c1[0], c = c2[0];
    float d = c0[1], e = c1[1], f = c2[1];
    float g = c0[2], h = c1[2], i = c2[2];

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
