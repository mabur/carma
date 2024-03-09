#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define let __auto_type // Requires GNUC. C23 also has auto.

#define RANGE(type) struct {type* data; size_t count;}
#define DARRAY(type) struct {type* data; size_t count; size_t capacity;}

#define COUNT(range) ((range).count)
#define CAPACITY(darray) ((darray).capacity)
#define LAST(range) ((range).data + (range).count)

#define INIT_RANGE(range, mycount) do { \
    (range).data = malloc((mycount) * sizeof(*(range).data)); \
    (range).count = (mycount); \
} while (0);

#define INIT_DARRAY(darray, mycount, mycapacity) do { \
    (darray).data = malloc((mycapacity) * sizeof(*(darray).data)); \
    (darray).count = (mycount); \
    (darray).capacity = (mycapacity); \
} while (0);

#define FREE_RANGE(range) do { \
    free((range).data); \
    (range).data = NULL; \
    (range).count = 0; \
} while (0);

#define FREE_DARRAY(darray) do { \
    free((darray).data); \
    (darray).data = NULL; \
    (darray).count = 0; \
    (darray).capacity = 0; \
} while (0);

// Requires GNUC:
#define MAKE_RANGE(value_type, ...) ({ \
    value_type array[] = { __VA_ARGS__ }; \
    size_t count = sizeof(array) / sizeof(value_type); \
    RANGE(value_type) result; \
    INIT_RANGE((result), count); \
    memcpy((result).data, array, count * sizeof(value_type)); \
    result; \
})

// Requires GNUC:
#define MAKE_DARRAY(value_type, ...) ({ \
    value_type array[] = { __VA_ARGS__ }; \
    size_t count = sizeof(array) / sizeof(value_type); \
    DARRAY(value_type) result; \
    INIT_DARRAY((result), count, count); \
    memcpy((result).data, array, count * sizeof(value_type)); \
    result; \
})

#define APPEND(darray, value) do { \
    if ((darray).count == (darray).capacity) { \
        size_t old_count = COUNT(darray); \
        size_t old_capacity = CAPACITY(darray); \
        size_t new_capacity = old_capacity == 0 ? 1 : 2 * old_capacity; \
        size_t item_size = sizeof(*(darray).data); \
        (darray).data = realloc((darray).data, new_capacity * item_size); \
        (darray).capacity = new_capacity; \
    } \
    ((darray).data)[(darray).count] = (value); \
    (darray).count++; \
} while (0)

#define FOR_EACH(it, range) \
    for (__auto_type it = (range).data; it != LAST(range); ++it)

#define CLEAR(range) do {(range).count = 0;} while (0)

// Requires GNUC:
#define SUM(range) ({ \
    typeof(*range.data) sum = 0; \
    FOR_EACH(it, (range)) { \
        sum += *it; \
    } \
    sum; \
})

// Requires GNUC:
#define MIN_ELEMENT(range) ({ \
    let minimum = (range).data; \
    FOR_EACH(it, (range)) { \
        minimum = *minimum > *it ? it : minimum; \
    } \
    minimum; \
})

// Requires GNUC:
#define MAX_ELEMENT(range) ({ \
    let maximum = (range).data; \
    FOR_EACH(it, (range)) { \
        maximum = *maximum < *it ? it : maximum; \
    } \
    maximum; \
})

// Requires GNUC:
#define EQUAL_RANGES(left_range, right_range) ({ \
    let result = true; \
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
    let it = (range).data; \
    for (; it != LAST(range) && !(predicate)(*it); ++it) { \
    } \
    it; \
})

// Requires GNUC:
#define FIND_IF_ITERATOR(first, last, predicate) ({ \
    let it = (first); \
    for (; it < last && !(predicate)(*it); ++it) { \
    } \
    it; \
})

// Requires GNUC:
#define FIND_IF_NOT(range, predicate) ({ \
    let it = (range).data; \
    for (; it != LAST(range) && (predicate)(*it); ++it) { \
    } \
    it; \
})

// Requires GNUC:
#define FIND_IF_BACKWARDS(range, predicate) ({ \
    let it = LAST(range) - 1; \
    for (; it != (range).data - 1 && !(predicate)(*it); --it) { \
    } \
    it; \
})

#define FIND_IF_NOT_BACKWARDS(range, predicate) ({ \
    let it = LAST(range) - 1; \
    for (; it != (range).data - 1 && (predicate)(*it); --it) { \
    } \
    it; \
})

#define FIND_IF_NOT_BACKWARDS_ITERATOR(first, last, predicate) ({ \
    let it = (last) - 1; \
    for (; it != (range).data - 1 && (predicate)(*it); --it) { \
    } \
    it; \
})

// Requires GNUC:
// TODO: think about capacity when calling this with a darray.
#define DROP_WHILE(range, predicate) ({ \
    let result = (range); \
    for (; result.count > 0 && (predicate)(*result.data); ++result.data, --result.count) { \
    } \
    result; \
})

// Requires GNUC:
// TODO: think about capacity when calling this with a darray.
#define DROP_UNTIL(range, predicate) ({ \
    let result = (range); \
    for (; result.count > 0 && !(predicate)(*result.data); ++result.data, --result.count) { \
    } \
    result; \
})

// Requires GNUC:
#define DROP_BACK_WHILE(range, predicate) ({ \
    let result = (range); \
    for (; COUNT(result) > 0 && (predicate)(*(LAST(result) - 1)); --result.count) { \
    } \
    result; \
})

// Requires GNUC:
#define DROP_BACK_UNTIL(range, predicate) ({ \
    let result = (range); \
    for (; COUNT(result) > 0 && !(predicate)(*(LAST(result) - 1)); --result.count) { \
    } \
    result; \
})

// Requires GNUC:
#define ERASE_IF(range, predicate) do { \
    let first_to_erase = (range).data; \
    let last_to_keep = (range).data + (range).count - 1; \
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
} while (0);

#define FOR_LINES(line, capacity, file) for (char line[capacity]; fgets(line, (capacity), (file)) != NULL;)
