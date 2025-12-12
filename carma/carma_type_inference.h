#pragma once

#if defined(__cplusplus) && __cplusplus >= 201103L
    #define CARMA_AUTO auto
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 202311L)
    #define CARMA_AUTO auto
#elif defined(__GNUC__)
    #define CARMA_AUTO __auto_type
#else
    #error "CARMA_AUTO requires any of these: C++11, C23, GCC, Clang"
#endif
