#pragma once

#include <stdio.h>
#include <stdlib.h>

#define CHECK_INTERNAL(condition) do { \
    if (!(condition)) { \
        fprintf(stderr, "CHECK_INTERNAL failed: %s (%s:%d)\n", \
            #condition, __FILE__, __LINE__); \
        fflush(stderr); \
        abort(); \
    } \
} while (0)

#define CHECK_EXTERNAL(condition, fmt, ...) do { \
    if (!(condition)) { \
        fprintf(stderr, "CHECK_EXTERNAL failed: %s (%s:%d): " fmt "\n", \
            #condition, __FILE__, __LINE__, ##__VA_ARGS__); \
        fflush(stderr); \
        exit(EXIT_FAILURE); \
    } \
} while (0)
