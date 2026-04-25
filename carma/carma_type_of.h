#pragma once

#if defined(__GNUC__) || defined(__clang__)
    #define CARMA_TYPE_OF(x) typeof(x)
#elif defined(__cplusplus)
    #define CARMA_TYPE_OF(x) decltype(x)
#endif
