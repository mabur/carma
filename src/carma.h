#include <stdlib.h>

#define let __auto_type // Requires GNUC. C23 also has auto.

#define RANGE(type) struct {type* first; type* last;}
#define DARRAY(type) struct {type* first; type* last; type* last_allocated;}

#define COUNT(range) ((range).last - (range).first)
#define CAPACITY(darray) ((darray).last_allocated - (darray).first)

#define INIT_RANGE(range, count) do { \
    (range).first = malloc((count) * sizeof(*(range).first)); \
    (range).last = (range).first + (count); \
} while (0);

#define INIT_DARRAY(darray, count, capacity) do { \
    (darray).first = malloc((capacity) * sizeof(*(darray).first)); \
    (darray).last = (darray).first + (count);      \
    (darray).last_allocated = (darray).first + (capacity); \
} while (0)

#define FREE_RANGE(range) do { \
    free((range).first); \
    (range).first = NULL; \
    (range).last = NULL; \
} while (0);

#define FREE_DARRAY(darray) do { \
    free((darray).first); \
    (darray).first = NULL; \
    (darray).last = NULL; \
    (darray).last_allocated = NULL; \
} while (0);

#define APPEND(darray, value) do { \
    if ((darray).last == (darray).last_allocated) { \
        size_t old_count = COUNT(darray); \
        size_t old_capacity = CAPACITY(darray); \
        size_t new_capacity = old_capacity == 0 ? 1 : 2 * old_capacity; \
        size_t item_size = sizeof(*(darray).first); \
        (darray).first = realloc((darray).first, new_capacity * item_size); \
        (darray).last = (darray).first + old_count; \
        (darray).last_allocated = (darray).first + new_capacity; \
    } \
    *(darray).last = (value); \
    (darray).last++; \
} while (0)

#define FOR_EACH(it, range) \
    for (__auto_type it = (range).first; it != (range).last; ++it)

#define CLEAR(range) do {(range).last = (range).first;} while (0)

// Requires GNUC:
#define SUM(range) ({ \
    typeof(*range.first) sum = 0; \
    FOR_EACH(it, (range)) { \
        sum += *it; \
    } \
    sum; \
})

// Requires GNUC:
#define MIN_ELEMENT(range) ({ \
    let minimum = (range).first; \
    FOR_EACH(it, (range)) { \
        minimum = *minimum > *it ? it : minimum; \
    } \
    minimum; \
})

// Requires GNUC:
#define MAX_ELEMENT(range) ({ \
    let maximum = (range).first; \
    FOR_EACH(it, (range)) { \
        maximum = *maximum < *it ? it : maximum; \
    } \
    maximum; \
})

// Requires GNUC:
#define FIND_IF(range, predicate) ({ \
    let it = (range).first; \
    for (; it != (range).last && !(predicate)(*it); ++it) { \
    } \
    it; \
})

// Requires GNUC:
#define FIND_IF_BACKWARDS(range, predicate) ({ \
    let it = (range).last - 1; \
    for (; it != (range).first - 1 && !(predicate)(*it); --it) { \
    } \
    it; \
})

// Requires GNUC:
#define DROP_WHILE(range, predicate) ({ \
    let result = (range); \
    for (; COUNT(result) > 0 && (predicate)(*result.first); ++result.first) { \
    } \
    result; \
})

// Requires GNUC:
#define DROP_UNTIL(range, predicate) ({ \
    let result = (range); \
    for (; COUNT(result) > 0 && !(predicate)(*result.first); ++result.first) { \
    } \
    result; \
})

// Requires GNUC:
#define DROP_BACK_WHILE(range, predicate) ({ \
    let result = (range); \
    for (; COUNT(result) > 0 && (predicate)(*(result.last - 1)); --result.last) { \
    } \
    result; \
})

// Requires GNUC:
#define DROP_BACK_UNTIL(range, predicate) ({ \
    let result = (range); \
    for (; COUNT(result) > 0 && !(predicate)(*(result.last - 1)); --result.last) { \
    } \
    result; \
})

// Requires GNUC:
#define ERASE_IF(range, predicate) do { \
    let sub_range = (range); \
    while (COUNT(sub_range) > 0) { \
        sub_range = DROP_UNTIL(sub_range, predicate); \
        sub_range = DROP_BACK_WHILE(sub_range, predicate); \
        if (COUNT(sub_range) > 1) { \
            *sub_range.first = *(sub_range.last - 1); \
            ++sub_range.first; \
            --sub_range.last; \
        } \
    } \
    (range).last = sub_range.last; \
} while (0);

#define FOR_LINES(line, capacity, file) for (char line[capacity]; fgets(line, (capacity), (file)) != NULL;)
