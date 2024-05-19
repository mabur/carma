#pragma once

#include <stdlib.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
// UTILITIES

#define END_POINTER(range) ((range).data + (range).count)

#define POINTER_TYPE(range) typeof((range).data)
#define VALUE_TYPE(range) typeof(*(range).data)

////////////////////////////////////////////////////////////////////////////////
// ALLOCATE MEMORY

#define INIT_RANGE(range, mycount) do { \
    (range).data = (POINTER_TYPE(range))malloc((mycount) * sizeof(VALUE_TYPE(range))); \
    (range).count = (mycount); \
} while (0)

#define INIT_DARRAY(darray, mycount, mycapacity) do { \
    (darray).data = (POINTER_TYPE(darray))malloc((mycapacity) * sizeof(VALUE_TYPE(darray))); \
    (darray).count = (mycount); \
    (darray).capacity = (mycapacity); \
} while (0)

#define INIT_IMAGE(image, mywidth, myheight) do { \
    (image).data = (POINTER_TYPE(image))malloc((mywidth) * (myheight) * sizeof(VALUE_TYPE(image))); \
    (image).width = (mywidth); \
    (image).height = (myheight); \
    (image).count = (mywidth) * (myheight); \
} while (0)

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

////////////////////////////////////////////////////////////////////////////////
// FREE MEMORY

#define FREE_RANGE(range) do { \
    free((range).data); \
    (range).data = NULL; \
    (range).count = 0; \
} while (0)

#define FREE_DARRAY(darray) do { \
    free((darray).data); \
    (darray).data = NULL; \
    (darray).count = 0; \
    (darray).capacity = 0; \
} while (0)

#define FREE_IMAGE(image) do { \
    free((image).data); \
    (image).data = NULL; \
    (image).width = 0; \
    (image).height = 0; \
    (image).count = 0; \
} while (0)

////////////////////////////////////////////////////////////////////////////////
// RANGE ALGORITHMS

#define FOR_EACH(iterator, range) \
    for (auto iterator = (range).data; iterator != END_POINTER(range); ++iterator)

#define FOR_EACH2(iterator0, iterator1, range0, range1) \
    auto iterator0 = (range0).data; \
    auto iterator1 = (range1).data; \
    for (; iterator0 != END_POINTER(range0) && iterator1 != END_POINTER(range1); ++iterator0, ++iterator1)

#define FOR_INDEX(index, range) \
    for (typeof((range).count) index = 0; index < (range).count; ++index)

#define FILL(range, value) FOR_EACH(it, (range)) *it = (value)

////////////////////////////////////////////////////////////////////////////////
// DYNAMIC ARRAY ALGORITHMS

#define NEXT_CAPACITY(capacity) (capacity) == 0 ? 1 : 2 * (capacity)

#define APPEND(dynamic_array, item) do { \
    if ((dynamic_array).count == (dynamic_array).capacity) { \
        (dynamic_array).capacity = NEXT_CAPACITY((dynamic_array).capacity); \
        (dynamic_array).data = (POINTER_TYPE(dynamic_array))realloc((dynamic_array).data, (dynamic_array).capacity * sizeof(VALUE_TYPE(dynamic_array))); \
    } \
    ((dynamic_array).data)[(dynamic_array).count] = (item); \
    (dynamic_array).count++; \
} while (0)

#define APPEND_ALL(dynamic_array, range) do { \
    auto new_count = (dynamic_array).count + (range).count; \
    if (new_count > (dynamic_array).capacity) { \
        while (new_count > (dynamic_array).capacity) { \
            (dynamic_array).capacity = NEXT_CAPACITY((dynamic_array).capacity); \
        } \
        (dynamic_array).data = (POINTER_TYPE(dynamic_array))realloc((dynamic_array).data, (dynamic_array).capacity * sizeof(VALUE_TYPE(dynamic_array))); \
    } \
    FOR_INDEX(i, (range)) { \
        ((dynamic_array).data)[(dynamic_array).count + i] = (range).data[i]; \
    } \
    (dynamic_array).count = new_count; \
} while (0)

#define CLEAR(dynamic_array) do {(dynamic_array).count = 0;} while (0)

#define ERASE_INDEX(dynamic_array, index) do { \
    --(dynamic_array).count; \
    if ((index) != (dynamic_array).count) { \
        (dynamic_array).data[index] = (dynamic_array).data[(dynamic_array).count]; \
    } \
} while (0)

#define ERASE_IF(dynamic_array, predicate) do { \
    auto a = (dynamic_array).data; \
    auto b = (dynamic_array).data + (dynamic_array).count; \
    while (a < b) { \
        for (; a < b && !(predicate)(*a); ++a) { \
        } \
        for (; a < b && (predicate)(*(b - 1)); --b) { \
        } \
        if (a + 1 < b) { \
            *a = *(b - 1); \
            ++a; \
            --b; \
        } \
    } \
    (dynamic_array).count = a - (dynamic_array).data; \
} while (0)

////////////////////////////////////////////////////////////////////////////////
// IMAGE ALGORITHMS

#define FOR_X(x, image) \
    for (typeof((image).width) x = 0; x < (image).width; ++x)

#define FOR_Y(y, image) \
    for (typeof((image).height) y = 0; y < (image).height; ++y)

////////////////////////////////////////////////////////////////////////////////
// FILE ALGORITHMS

#define FOR_LINES(line, capacity, file) for (char line[capacity]; fgets(line, (capacity), (file)) != NULL;)
