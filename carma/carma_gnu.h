#pragma once

#include "carma.h"

////////////////////////////////////////////////////////////////////////////////
// ALGORITHMS FOR ALLOCATION AND FREE

#define MAKE_RANGE(value_type, ...) ({ \
    RANGE(value_type) result; \
    INIT_RANGE_ELEMENTS(result, __VA_ARGS__); \
    result; \
})

#define MAKE_DARRAY(value_type, ...) ({ \
    DARRAY(value_type) result; \
    INIT_DARRAY_ELEMENTS(result, __VA_ARGS__); \
    result; \
})

////////////////////////////////////////////////////////////////////////////////
// RANGE ALGORITHMS

#define SUM(range) ({ \
    typeof(*(range).data) sum = 0; \
    FOR_EACH(it, (range)) { \
        sum += *it; \
    } \
    sum; \
})

#define MIN_ELEMENT(range) ({ \
    auto minimum = (range).data; \
    FOR_EACH(it, (range)) { \
        minimum = *minimum > *it ? it : minimum; \
    } \
    minimum; \
})

#define MAX_ELEMENT(range) ({ \
    auto maximum = (range).data; \
    FOR_EACH(it, (range)) { \
        maximum = *maximum < *it ? it : maximum; \
    } \
    maximum; \
})

#define EQUAL_RANGES(left_range, right_range) ({ \
    auto result = true; \
    if ((left_range).count != (right_range).count) { \
        result = false; \
    } else { \
        for (size_t i = 0; i < (left_range).count; ++i) { \
            if ((left_range).data[i] != (right_range).data[i]) { \
                result = false; \
                break; \
            } \
        } \
    } \
    result; \
})

#define FIND_IF(range, predicate) ({ \
    auto it = (range).data; \
    for (; it != END_POINTER(range) && !(predicate)(*it); ++it) { \
    } \
    it; \
})

#define FIND_IF_ITERATOR(first, last, predicate) ({ \
    auto it = (first); \
    for (; it < last && !(predicate)(*it); ++it) { \
    } \
    it; \
})

#define FIND_IF_NOT(range, predicate) ({ \
    auto it = (range).data; \
    for (; it != END_POINTER(range) && (predicate)(*it); ++it) { \
    } \
    it; \
})

#define FIND_IF_BACKWARDS(range, predicate) ({ \
    auto it = END_POINTER(range) - 1; \
    for (; it != (range).data - 1 && !(predicate)(*it); --it) { \
    } \
    it; \
})

#define FIND_IF_NOT_BACKWARDS(range, predicate) ({ \
    auto it = END_POINTER(range) - 1; \
    for (; it != (range).data - 1 && (predicate)(*it); --it) { \
    } \
    it; \
})

#define FIND_IF_NOT_BACKWARDS_ITERATOR(first, last, predicate) ({ \
    auto it = (last) - 1; \
    for (; it != (range).data - 1 && (predicate)(*it); --it) { \
    } \
    it; \
})

////////////////////////////////////////////////////////////////////////////////
// DYNAMIC ARRAY ALGORITHMS

// TODO: think about capacity when calling this with a darray.
#define DROP_WHILE(range, predicate) ({ \
    auto result = (range); \
    for (; result.count > 0 && (predicate)(*result.data); ++result.data, --result.count) { \
    } \
    result; \
})

// TODO: think about capacity when calling this with a darray.
#define DROP_UNTIL(range, predicate) ({ \
    auto result = (range); \
    for (; result.count > 0 && !(predicate)(*result.data); ++result.data, --result.count) { \
    } \
    result; \
})

#define DROP_BACK_WHILE(range, predicate) ({ \
    auto result = (range); \
    for (; (result).count > 0 && (predicate)(*(END_POINTER(result) - 1)); --result.count) { \
    } \
    result; \
})

#define DROP_BACK_UNTIL(range, predicate) ({ \
    auto result = (range); \
    for (; (result).count > 0 && !(predicate)(*(END_POINTER(result) - 1)); --result.count) { \
    } \
    result; \
})
