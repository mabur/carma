#pragma once

#include <stdbool.h>

#include "carma.h"

////////////////////////////////////////////////////////////////////////////////
// ALGORITHMS FOR ALLOCATION AND FREE

#define RANGE(type) struct {type* data; size_t count;}
#define DARRAY(type) struct {type* data; size_t count; size_t capacity;}

#define INIT_RANGE_ELEMENTS(range, ...) do { \
    VALUE_TYPE(range) array[] = { __VA_ARGS__ }; \
    INIT_RANGE((range), sizeof(array) / sizeof(VALUE_TYPE(range))); \
    memcpy((range).data, array, sizeof(array)); \
} while (0)

#define INIT_DARRAY_ELEMENTS(range, ...) do { \
    VALUE_TYPE(range) array[] = { __VA_ARGS__ }; \
    size_t c = sizeof(array) / sizeof(VALUE_TYPE(range)); \
    INIT_DARRAY((range), c, c); \
    memcpy((range).data, array, sizeof(array)); \
} while (0)

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

#define EQUAL_RANGES(left_range, right_range) ({ \
    auto result = true; \
    if ((left_range).count != (right_range).count) { \
        result = false; \
    } else { \
        FOR_EACH2(left, right, (left_range), (right_range)) { \
            if (*left != *right) { \
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
