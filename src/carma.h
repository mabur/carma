#include <stdlib.h>

#define let __auto_type // Requires GNUC. C23 also has auto.

#define INIT_RANGE(range, size_) do { \
    (range).data = malloc((size_) * sizeof(*(range).data)); \
    (range).size = (size_); \
} while (0);

#define INIT_DARRAY(range, size_, capacity_) do { \
    (range).data = malloc((capacity_) * sizeof(*(range).data)); \
    (range).size = (size_); \
    (range).capacity = (capacity_); \
} while (0);

#define FREE_RANGE(range) do { \
    free((range).data); \
    (range).data = NULL; \
} while (0);

#define FOR_RANGE(it, range) \
    for (__auto_type it = (range).data; it != (range).data + (range).size; ++it)

#define APPEND(range, value) do { \
    if ((range).size >= (range).capacity) exit(EXIT_FAILURE); \
    *((range).data + (range).size) = (value); \
    (range).size++; \
} while (0);

// Requires GNUC:
#define SUM_RANGE(range) ({ \
    typeof(*range.data) sum = 0; \
    FOR_RANGE(it, (range)) { \
        sum += *it; \
    } \
    sum; \
})

// Requires GNUC:
#define MAX_ELEMENT_RANGE(range) ({ \
    typeof(range.data) maximum = range.data; \
    FOR_RANGE(it, (range)) { \
        maximum = *maximum < *it ? it : maximum; \
    } \
    maximum; \
})
