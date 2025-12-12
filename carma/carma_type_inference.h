#pragma once

#if defined(__cplusplus)
    #if __cplusplus >= 201103L
        #define CARMA_AUTO auto
    #else
        #error "CARMA_AUTO requires any of these: C23, C++11, GCC, Clang"
    #endif
#else
    #if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 202311L)
        #define CARMA_AUTO auto
    #elif defined(__clang__) || defined(__GNUC__)
        #define CARMA_AUTO __auto_type
    #else
        #error "CARMA_AUTO requires any of these: C23, C++11, GCC, Clang"
    #endif
#endif
