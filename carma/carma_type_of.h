#pragma once

#if defined(__GNUC__) || defined(__clang__)
    #define TYPE_OF_EXPRESSION(x) typeof(x)
#elif defined(__cplusplus)
    #define TYPE_OF_EXPRESSION(x) decltype(x)
#endif
