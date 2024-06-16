#pragma once

#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "carma.h"

// Naming: StringLiteral, ConstantString, StringView...
typedef struct ConstantString {
    const char* data;
    size_t count;
} ConstantString;

typedef struct DynamicString {
    char* data;
    size_t count;
    size_t capacity;
} DynamicString;

static
inline
ConstantString makeConstantString(const char* s) {
    return (ConstantString){s, strlen(s)};
}

static
inline
DynamicString concatenate(DynamicString base, const char* tail) {
    CONCAT(base, makeConstantString(tail));
    return base;
}

static
inline
DynamicString carmaFormatString(DynamicString string, const char* format, ...) {
    va_list args;
    va_start(args, format);
    auto num_characters = vsnprintf(string.data, (size_t)string.capacity, format, args);
    if (num_characters >= 0) {
        if (num_characters >= (int)string.capacity) {
            RESERVE(string, (size_t) num_characters + 1);
            num_characters = vsnprintf(string.data, string.capacity, format, args);
        }
        string.count = (size_t)num_characters;
    }
    va_end(args);
    return string;
}

#define FORMAT_STRING(string, format, ...) do { \
    (string) = carmaFormatString((string), (format), ##__VA_ARGS__); \
} while(0)
