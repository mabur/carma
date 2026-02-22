#pragma once

#include <stdio.h>
#include <stdlib.h>

#define CARMA_EXIT_FAILURE(fmt, ...) ( \
    fprintf(stderr, fmt, ##__VA_ARGS__), \
    fflush(stderr), \
    exit(EXIT_FAILURE) \
)

#define CARMA_ABORT_FAILURE(fmt, ...) ( \
    fprintf(stderr, fmt, ##__VA_ARGS__), \
    fflush(stderr), \
    abort() \
)

#define CHECK_INTERNAL(condition) do { \
    if (!(condition)) { \
        CARMA_ABORT_FAILURE("CHECK_INTERNAL failed at %s:%d\n", __FILE__, __LINE__); \
    } \
} while (0)

#define CHECK_EXTERNAL(condition, fmt, ...) do { \
    if (!(condition)) { \
        CARMA_EXIT_FAILURE("CHECK_EXTERNAL failed at %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    } \
} while (0)

#define CHECK_INTERNAL_VALUE(value, condition) \
    ((condition) ? (value) \
        : (CARMA_ABORT_FAILURE("CHECK_INTERNAL failed at %s:%d\n", __FILE__, __LINE__), \
            (value)))

#define CHECK_EXTERNAL_VALUE(value, condition, fmt, ...) \
    ((condition) ? (value) \
        : (fprintf(stderr, "CHECK_EXTERNAL failed at %s:%d:\n", __FILE__, __LINE__), \
            CARMA_EXIT_FAILURE(fmt, ##__VA_ARGS__), \
            (value)))
