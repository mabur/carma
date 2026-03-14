#pragma once

// Define a "make" macro for brace/compound initialization,
// that is easy to type and works across C/C++ versions.
// It unifies these features:
// Compound Literals (C99), Designated Initializers (C99)
// Aggregate Initialization (C++98), Designated Initializers (C++20)
#if defined(__cplusplus)
    #define MAKE(type, ...) type{__VA_ARGS__}
#else
    #define MAKE(type, ...) ((type){__VA_ARGS__})
#endif
