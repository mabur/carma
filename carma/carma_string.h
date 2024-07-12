#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "carma.h"

typedef struct ConstantString {
    const char* data;
    size_t count;
} ConstantString;

typedef struct DynamicString {
    char* data;
    size_t count;
    size_t capacity;
} DynamicString;

#define CONSTANT_STRING(s) (ConstantString){(s), strlen(s)}

static
inline
size_t carmaRoundUpToPowerOf2(size_t x) {
    if (x == 0) {
        return 1;
    }
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    // Handle 64 bit size_t:
    if (sizeof(size_t) > 4) {
        x |= x >> 32;
    }
    return x + 1;
}

static
inline
DynamicString carmaFormatString(DynamicString string, const char* format, ...) {
    va_list args0;
    va_list args1;
    va_start(args0, format);
    va_copy(args1, args0);
    
    // Try writing at the end of the string:
    auto num_characters = vsnprintf(
        END_POINTER(string),
        REMAINING_CAPACITY(string),
        format,
        args0
    );
    // Check if we succeed:
    if (num_characters >= 0) {
        size_t required_capacity = string.count + (size_t)num_characters + 1;
        // Check if we need to reallocate the string to fit:
        if (required_capacity > string.capacity) {
            size_t new_capacity = carmaRoundUpToPowerOf2(required_capacity);
            RESERVE(string, new_capacity);
            // Write the string that should fit now:
            num_characters = vsnprintf(
                END_POINTER(string), REMAINING_CAPACITY(string), format, args1
            );
        }
        string.count += (size_t)num_characters;
    }

    va_end(args0);
    va_end(args1);
    return string;
}

#define FORMAT_STRING(string, format, ...) do { \
    (string) = carmaFormatString((string), (format), ##__VA_ARGS__); \
} while(0)
