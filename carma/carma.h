#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

////////////////////////////////////////////////////////////////////////////////
// UTILITIES

#define IS_EMPTY(range) ((range).count == 0)

#define BEGIN_POINTER(range) ((range).data)
#define END_POINTER(range) ((range).data + (range).count)

#define FIRST_ITEM(range) ((range).data[0])
#define LAST_ITEM(range) ((range).data[0 + (range).count - 1])

#define REMAINING_CAPACITY(darray) ((darray).capacity + (darray).count)

#define ITEM_SIZE(range) sizeof(*(range).data)

#define SWAP(a, b) do { \
    auto c = (a); \
    (a) = (b); \
    (b) = c; \
} while (0)


////////////////////////////////////////////////////////////////////////////////
// MEMBER TYPES

#if defined(__GNUC__) || defined(__clang__)
    #define TYPE_OF_EXPRESSION(x) typeof(x)
#elif defined(__cplusplus)
    #define TYPE_OF_EXPRESSION(x) decltype(x)
#endif

#define POINTER_TYPE(range) TYPE_OF_EXPRESSION((range).data)
#define VALUE_TYPE(range) TYPE_OF_EXPRESSION(*(range).data)
#define INDEX_TYPE(range) TYPE_OF_EXPRESSION((range).count)

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

#define INIT_2D_ARRAY(array, mywidth, myheight) do { \
    (array).data = (POINTER_TYPE(array))malloc((mywidth) * (myheight) * sizeof(VALUE_TYPE(array))); \
    (array).width = (mywidth); \
    (array).height = (myheight); \
    (array).count = (mywidth) * (myheight); \
} while (0)

#define INIT_3D_ARRAY(array, mywidth, myheight, mydepth) do { \
    (array).data = (POINTER_TYPE(array))malloc((mywidth) * (myheight) * (mydepth) * sizeof(VALUE_TYPE(array))); \
    (array).width = (mywidth); \
    (array).height = (myheight); \
    (array).depth = (mydepth); \
    (array).count = (mywidth) * (myheight) * (mydepth); \
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

#define FREE_2D_ARRAY(array) do { \
    free((array).data); \
    (array).data = NULL; \
    (array).width = 0; \
    (array).height = 0; \
    (array).count = 0; \
} while (0)

#define FREE_3D_ARRAY(array) do { \
    free((array).data); \
    (array).data = NULL; \
    (array).width = 0; \
    (array).height = 0; \
    (array).depth = 0; \
    (array).count = 0; \
} while (0)

////////////////////////////////////////////////////////////////////////////////
// RANGE ALGORITHMS

#define FOR_EACH(iterator, range) \
    for (auto iterator = BEGIN_POINTER(range); iterator != END_POINTER(range); ++iterator)

#define FOR_EACH_BACKWARD(iterator, range) \
    for (auto iterator = END_POINTER(range); iterator != BEGIN_POINTER(range);) \
        if (--iterator || true)
    
#define FOR_EACH2(iterator0, iterator1, range0, range1) \
    auto iterator0 = BEGIN_POINTER(range0); \
    auto iterator1 = BEGIN_POINTER(range1); \
    for (; iterator0 != END_POINTER(range0) && iterator1 != END_POINTER(range1); ++iterator0, ++iterator1)

#define FOR_EACH3(iterator0, iterator1, iterator2, range0, range1, range2) \
    auto iterator0 = BEGIN_POINTER(range0); \
    auto iterator1 = BEGIN_POINTER(range1); \
    auto iterator2 = BEGIN_POINTER(range2); \
    for (; iterator0 != END_POINTER(range0) && iterator1 != END_POINTER(range1) && iterator2 != END_POINTER(range2); ++iterator0, ++iterator1, ++iterator2)
    
#define FOR_INDEX(index, range) \
    for (INDEX_TYPE(range) index = 0; index < (range).count; ++index)

#define FILL(range, value) FOR_EACH(it, (range)) *it = (value)

#define FOR_MIN(it, range) \
    auto it = (range).data; \
    FOR_EACH(internal_it, (range)) { \
        it = *it < *internal_it ? it : internal_it; \
    } \
    if (it != END_POINTER(range))

#define FOR_MAX(it, range) \
    auto it = (range).data; \
    FOR_EACH(internal_it, (range)) { \
        it = *it < *internal_it ? internal_it : it; \
    } \
    if (it != END_POINTER(range))


#define COUNT_BYTES(range) (sizeof(VALUE_TYPE(range)) * (range).count)
    
static inline bool carma_are_bits_equal(
    const void* data0,
    const void* data1,
    size_t byte_count0,
    size_t byte_count1
) {
    if (byte_count0 != byte_count1) {
        return false;
    }
    return memcmp(data0, data1, byte_count0) == 0;
}

#define ARE_EQUAL(range0, range1) \
    carma_are_bits_equal((range0).data, (range1).data, COUNT_BYTES(range0), COUNT_BYTES(range1))

////////////////////////////////////////////////////////////////////////////////
// RANGE ALGORITHMS - DROP
// TODO: think about capacity when calling drop functions with a darray.
// Document behavior.

#define DROP_FRONT(range) do { \
    (range).data++; \
    (range).count--; \
} while (0)

#define DROP_BACK(range) do { \
    (range).count--; \
} while (0)

#define DROP_FRONT_WHILE_ITEM(range, item) \
    while (!IS_EMPTY(range) && FIRST_ITEM(range) == item) { \
        DROP_FRONT(range); \
    }

#define DROP_FRONT_UNTIL_ITEM(range, item) \
    while (!IS_EMPTY(range) && FIRST_ITEM(range) != item) { \
        DROP_FRONT(range); \
    }

#define DROP_BACK_WHILE_ITEM(range, item) \
    while (!IS_EMPTY(range) && LAST_ITEM(range) == item) { \
        DROP_BACK(range); \
    }

#define DROP_BACK_UNTIL_ITEM(range, item) \
    while (!IS_EMPTY(range) && LAST_ITEM(range) != item) { \
        DROP_BACK(range); \
    }

#define DROP_FRONT_WHILE(range, predicate) \
    while (!IS_EMPTY(range) && (predicate)(FIRST_ITEM(range))) { \
        DROP_FRONT(range); \
    }

#define DROP_FRONT_UNTIL(range, predicate) \
    while (!IS_EMPTY(range) && !(predicate)(FIRST_ITEM(range))) { \
        DROP_FRONT(range); \
    }

#define DROP_BACK_WHILE(range, predicate) \
    while (!IS_EMPTY(range) && (predicate)(LAST_ITEM(range))) { \
        DROP_BACK(range); \
    }

#define DROP_BACK_UNTIL(range, predicate) \
    while (!IS_EMPTY(range) && !(predicate)(LAST_ITEM(range))) { \
        DROP_BACK(range); \
    }

////////////////////////////////////////////////////////////////////////////////
// DYNAMIC ARRAY ALGORITHMS

#define RESERVE(dynamic_array, new_capacity) do { \
    (dynamic_array).capacity = (new_capacity); \
    if ((dynamic_array).count > (new_capacity)) { \
        (dynamic_array).count = (new_capacity); \
    } \
    (dynamic_array).data = (POINTER_TYPE(dynamic_array))realloc((dynamic_array).data, (new_capacity) * sizeof(VALUE_TYPE(dynamic_array))); \
} while (0)

#define RESERVE_EXPONENTIAL_GROWTH(dynamic_array, min_required_capacity) do { \
    while ((dynamic_array).capacity < (min_required_capacity)) { \
        if ((dynamic_array).capacity == 0) { \
            (dynamic_array).capacity = 1; \
        } \
        else { \
            (dynamic_array).capacity *= 2; \
        } \
    } \
    RESERVE((dynamic_array), (dynamic_array).capacity); \
} while (0)

#define APPEND(dynamic_array, item) do { \
    auto new_count = (dynamic_array).count + 1; \
    if (new_count > (dynamic_array).capacity) { \
        RESERVE_EXPONENTIAL_GROWTH((dynamic_array), new_count); \
    } \
    ((dynamic_array).data)[(dynamic_array).count] = (item); \
    (dynamic_array).count++; \
} while (0)

#define PREPEND(dynamic_array, item) INSERT_INDEX((dynamic_array), 0, (item))

#define CONCAT(dynamic_array, range) do { \
    auto new_count = (dynamic_array).count + (range).count; \
    if (new_count > (dynamic_array).capacity) { \
        RESERVE_EXPONENTIAL_GROWTH((dynamic_array), new_count); \
    } \
    FOR_INDEX(i, (range)) { \
        ((dynamic_array).data)[(dynamic_array).count + i] = (range).data[i]; \
    } \
    (dynamic_array).count = new_count; \
} while (0)

#define INSERT_INDEX(dynamic_array, index, item) do { \
    APPEND((dynamic_array), (item)); \
    for (auto i = (dynamic_array).count - 1; i > (index); --i) { \
        (dynamic_array).data[i] = (dynamic_array).data[i - 1]; \
    } \
    (dynamic_array).data[index] = (item); \
} while (0)

#define CLEAR(dynamic_array) do {(dynamic_array).count = 0;} while (0)

#define ERASE_INDEX(dynamic_array, index) do { \
    --(dynamic_array).count; \
    if ((index) != (dynamic_array).count) { \
        (dynamic_array).data[index] = (dynamic_array).data[(dynamic_array).count]; \
    } \
} while (0)

#define ERASE_INDEX_ORDERED(dynamic_array, index) { \
    for (INDEX_TYPE(dynamic_array) i = index; i + 1 < (dynamic_array).count; ++i) { \
        (dynamic_array).data[i] = (dynamic_array).data[i + 1]; \
    } \
    ERASE_BACK(dynamic_array); \
} while (0)

#define ERASE_BACK(dynamic_array) do { \
    (dynamic_array).count--; \
} while (0)

#define ERASE_FRONT(dynamic_array) ERASE_INDEX_ORDERED((dynamic_array), 0)

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
// MULTI DIMENSIONAL ARRAY ALGORITHMS

#define FOR_X(x, array) \
    for (INDEX_TYPE(array) x = 0; x < (array).width; ++x)

#define FOR_Y(y, array) \
    for (INDEX_TYPE(array) y = 0; y < (array).height; ++y)

#define FOR_Z(z, array) \
    for (INDEX_TYPE(array) z = 0; z < (array).depth; ++z)

#define AT_XY(array, x, y) \
    ((array).data[(array).width * (y) + (x)])

#define AT_XYZ(array, x, y, z) \
    ((array).data[(array).height * (array).width * (z) + (array).width * (y) + (x)])

#define FLIP_IMAGE_X(image) do { \
    auto width = (image).width; \
    auto height = (image).height; \
    for (INDEX_TYPE(image) y = 0; y < height; ++y) \
        for (INDEX_TYPE(image) x = 0; x < width / 2; ++x) \
            SWAP(AT_XY((image), x, y), AT_XY((image), width - 1 - x, y)); \
} while(0)

#define FLIP_IMAGE_Y(image) do { \
    auto width = (image).width; \
    auto height = (image).height; \
    for (INDEX_TYPE(image) y = 0; y < height / 2; ++y) \
        for (INDEX_TYPE(image) x = 0; x < width; ++x) \
            SWAP(AT_XY((image), x, y), AT_XY((image), x, height - 1 - y)); \
} while(0)
