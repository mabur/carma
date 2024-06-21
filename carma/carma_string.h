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
DynamicString carmaFormatString(DynamicString string, const char* format, ...) {
    va_list args0;
    va_list args1;
    va_start(args0, format);
    va_copy(args1, args0);
    
    auto num_characters = vsnprintf(string.data, (size_t)string.capacity, format, args0);
    if (num_characters >= 0) {
        if (num_characters >= (int)string.capacity) {
            RESERVE(string, (size_t) num_characters + 1);
            num_characters = vsnprintf(string.data, string.capacity, format, args1);
        }
        string.count = (size_t)num_characters;
    }

    va_end(args0);
    va_end(args1);
    return string;
}

#define FORMAT_STRING(string, format, ...) do { \
    (string) = carmaFormatString((string), (format), ##__VA_ARGS__); \
} while(0)
