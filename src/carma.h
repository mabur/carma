#include <stdlib.h>

#define let __auto_type // Requires GNUC. C23 also has auto.

#define RANGE(type) struct {type* first; type* last;}
#define DARRAY(type) struct {type* first; type* last; type* last_allocated;}

#define INIT_RANGE(range, count) do { \
    (range).first = malloc((count) * sizeof(*(range).first)); \
    (range).last = (range).first + (count); \
} while (0);

#define FREE_RANGE(range) do { \
    free((range).first); \
    (range).first = NULL; \
    (range).last = NULL; \
} while (0);

#define FOR_EACH(it, range) \
    for (__auto_type it = (range).first; it != (range).last; ++it)

#define INIT_DARRAY(range, count, capacity) do { \
    (range).first = malloc((capacity) * sizeof(*(range).first)); \
    (range).last = (range).first + (count);      \
    (range).last_allocated = (range).first + (capacity); \
} while (0)

#define FREE_DARRAY(range) do { \
    free((range).first); \
    (range).first = NULL; \
    (range).last = NULL; \
    (range).last_allocated = NULL; \
} while (0);

#define APPEND(range, value) do { \
    if ((range).last >= (range).last_allocated) exit(EXIT_FAILURE); \
    *(range).last = (value); \
    (range).last++; \
} while (0)

#define CLEAR_DARRAY(range) do {(range).last = (range).first;} while (0)

// Requires GNUC:
#define SUM_RANGE(range) ({ \
    typeof(*range.first) sum = 0; \
    FOR_EACH(it, (range)) { \
        sum += *it; \
    } \
    sum; \
})

// Requires GNUC:
#define MIN_ELEMENT_RANGE(range) ({ \
    let minimum = (range).first; \
    FOR_EACH(it, (range)) { \
        minimum = *minimum > *it ? it : minimum; \
    } \
    minimum; \
})

// Requires GNUC:
#define MAX_ELEMENT_RANGE(range) ({ \
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

#define FIND_IF_BACKWARDS(range, predicate) ({ \
    let it = (range).last - 1; \
    for (; it != (range).first - 1 && !(predicate)(*it); --it) { \
    } \
    it; \
})

#define COUNT(range) ((range).last - (range).first)

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

#define ERASE_IF(darray, predicate) do { \
    let range = (darray); \
    while (COUNT(range) > 0) { \
        range = DROP_UNTIL(range, predicate); \
        range = DROP_BACK_WHILE(range, predicate); \
        if (COUNT(range) > 1) { \
            *range.first = *(range.last - 1); \
            ++range.first; \
            --range.last; \
        } \
    } \
    (darray).last = range.last; \
} while (0);

#define FOR_LINES(line, capacity, file) for (char line[capacity]; fgets(line, (capacity), (file)) != NULL;)
