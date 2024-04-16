#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define RANGE(type) struct {type* data; size_t count;}
#define DARRAY(type) struct {type* data; size_t count; size_t capacity;}

#define END_POINTER(range) ((range).data + (range).count)

#define POINTER_TYPE(range) typeof((range).data)
#define VALUE_TYPE(range) typeof(*(range).data)

#define INIT_RANGE(range, mycount) do { \
    (range).data = (POINTER_TYPE(range))malloc((mycount) * sizeof(VALUE_TYPE(range))); \
    (range).count = (mycount); \
} while (0)

#define INIT_DARRAY(darray, mycount, mycapacity) do { \
    (darray).data = (POINTER_TYPE(darray))malloc((mycapacity) * sizeof(VALUE_TYPE(darray))); \
    (darray).count = (mycount); \
    (darray).capacity = (mycapacity); \
} while (0)

#define INIT_IMAGE(image, width, height) do { \
    (image).data = (POINTER_TYPE(image))malloc((width) * (height) * sizeof(VALUE_TYPE(image))); \
    (image).width = (width); \
    (image).height = (height); \
    (image).count = (width) * (height); \
} while (0)

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

// Requires GNUC:
#define MAKE_RANGE(value_type, ...) ({ \
    RANGE(value_type) result; \
    INIT_RANGE_ELEMENTS(result, __VA_ARGS__); \
    result; \
})

// Requires GNUC:
#define MAKE_DARRAY(value_type, ...) ({ \
    DARRAY(value_type) result; \
    INIT_DARRAY_ELEMENTS(result, __VA_ARGS__); \
    result; \
})

#define APPEND(darray, value) do { \
    if ((darray).count == (darray).capacity) { \
        (darray).capacity = (darray).capacity == 0 ? 1 : 2 * (darray).capacity; \
        (darray).data = realloc((darray).data, (darray).capacity * sizeof(*(darray).data)); \
    } \
    ((darray).data)[(darray).count] = (value); \
    (darray).count++; \
} while (0)

#define FOR_EACH(it, range) \
    for (auto it = (range).data; it != END_POINTER(range); ++it)

#define ENUMERATE(i, range) \
    for (size_t i = 0; i < (range).count; ++i)

#define CLEAR(range) do {(range).count = 0;} while (0)

// Requires GNUC:
#define SUM(range) ({ \
    typeof(*(range).data) sum = 0; \
    FOR_EACH(it, (range)) { \
        sum += *it; \
    } \
    sum; \
})

// Requires GNUC:
#define MIN_ELEMENT(range) ({ \
    auto minimum = (range).data; \
    FOR_EACH(it, (range)) { \
        minimum = *minimum > *it ? it : minimum; \
    } \
    minimum; \
})

// Requires GNUC:
#define MAX_ELEMENT(range) ({ \
    auto maximum = (range).data; \
    FOR_EACH(it, (range)) { \
        maximum = *maximum < *it ? it : maximum; \
    } \
    maximum; \
})

// Requires GNUC:
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

// Requires GNUC:
#define FIND_IF(range, predicate) ({ \
    auto it = (range).data; \
    for (; it != END_POINTER(range) && !(predicate)(*it); ++it) { \
    } \
    it; \
})

// Requires GNUC:
#define FIND_IF_ITERATOR(first, last, predicate) ({ \
    auto it = (first); \
    for (; it < last && !(predicate)(*it); ++it) { \
    } \
    it; \
})

// Requires GNUC:
#define FIND_IF_NOT(range, predicate) ({ \
    auto it = (range).data; \
    for (; it != END_POINTER(range) && (predicate)(*it); ++it) { \
    } \
    it; \
})

// Requires GNUC:
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

// Requires GNUC:
// TODO: think about capacity when calling this with a darray.
#define DROP_WHILE(range, predicate) ({ \
    auto result = (range); \
    for (; result.count > 0 && (predicate)(*result.data); ++result.data, --result.count) { \
    } \
    result; \
})

// Requires GNUC:
// TODO: think about capacity when calling this with a darray.
#define DROP_UNTIL(range, predicate) ({ \
    auto result = (range); \
    for (; result.count > 0 && !(predicate)(*result.data); ++result.data, --result.count) { \
    } \
    result; \
})

// Requires GNUC:
#define DROP_BACK_WHILE(range, predicate) ({ \
    auto result = (range); \
    for (; (result).count > 0 && (predicate)(*(END_POINTER(result) - 1)); --result.count) { \
    } \
    result; \
})

// Requires GNUC:
#define DROP_BACK_UNTIL(range, predicate) ({ \
    auto result = (range); \
    for (; (result).count > 0 && !(predicate)(*(END_POINTER(result) - 1)); --result.count) { \
    } \
    result; \
})

#define ERASE_IF(range, predicate) do { \
    auto first_to_erase = (range).data; \
    auto last_to_keep = (range).data + (range).count - 1; \
    while (first_to_erase <= last_to_keep) { \
        for (; first_to_erase <= last_to_keep && !(predicate)(*first_to_erase); ++first_to_erase) { \
        } \
        for (; first_to_erase <= last_to_keep && (predicate)(*last_to_keep); --last_to_keep) { \
        } \
        if (first_to_erase < last_to_keep) { \
            *first_to_erase = *last_to_keep; \
            ++first_to_erase; \
            --last_to_keep; \
        } \
    } \
    (range).count = first_to_erase - (range).data; \
} while (0)

#define FOR_LINES(line, capacity, file) for (char line[capacity]; fgets(line, (capacity), (file)) != NULL;)
