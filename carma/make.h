#pragma once

// Define a "let" macro for type inference,
// that is easy to type and works across C/C++ versions:
#if defined(__cplusplus) && __cplusplus >= 201103L
    #define let auto
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
    #define let auto
#elif defined(__clang__) && __has_extension(__auto_type)
    #define let __auto_type
#elif defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 9))
    #define let __auto_type
#else
    #error "The 'let' macro requires C++11, C23, or compiler support for __auto_type"
#endif

// Define a "make" macro for making "struct literals",
// that is easy to type and works across C/C++ versions.
// It unifies these features:
// Compound Literals (C99), Designated Initializers (C99)
// Aggregate Initialization (C++98), Designated Initializers (C++20)
#if defined(__cplusplus)
    #define make(type, ...) type{__VA_ARGS__}
#else
    #define make(type, ...) ((type){__VA_ARGS__})
#endif
