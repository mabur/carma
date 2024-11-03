#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "carma.h"

typedef struct StringView {
    const char* data;
    size_t count;
} StringView;

typedef struct DynamicString {
    char* data;
    size_t count;
    size_t capacity;
} DynamicString;

#define STRING_VIEW(cstring) (StringView){(cstring), strlen(cstring)}

#define CONCAT_CSTRING(dynamic_string, cstring) do { \
    StringView tail_with_null_terminator = {(cstring), strlen(cstring) + 1}; \
    CONCAT((dynamic_string), tail_with_null_terminator); \
    DROP_BACK(dynamic_string); \
} while (0)

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
            RESERVE_EXPONENTIAL_GROWTH(string, required_capacity);
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
} while (0)


static
inline
size_t _find_first_character_of(const char* data, size_t max_index, int (*predicate)(int)) {
    for (size_t i = 0; i < max_index; ++i) {
        if (predicate(data[i])) {
            return i;
        }
    }
    return max_index;
}

static
inline
size_t _find_first_character_not_of(const char* data, size_t max_index, int (*predicate)(int)) {
    for (size_t i = 0; i < max_index; ++i) {
        if (!predicate(data[i])) {
            return i;
        }
    }
    return max_index;
}

#define FOR_EACH_WORD(word, string, is_delimeter) \
    for (\
    size_t _i\
    ;\
    _i = _find_first_character_of((string).data, (string).count, (is_delimeter)),\
    (word).data = whole.data,\
    (word).count = _i,\
    (string).data += _i,\
    (string).count -= _i,\
    !IS_EMPTY(part)\
    ;\
    _i = _find_first_character_not_of((string).data, (string).count, (is_delimeter)),\
    (string).data += _i,\
    (string).count -= _i\
    )
    
////////////////////////////////////////////////////////////////////////////////
// FILE ALGORITHMS

#define FOR_FILE(file, filepath, mode) for (auto (file) = fopen((file_path), mode); (file); fclose(file), (file) = 0)

#define FOR_LINES(line, capacity, file) for (char line[capacity]; fgets(line, (capacity), (file)) != NULL;)

#define READ_LINES(line, capacity, file_path) \
    FOR_FILE(_file, file_path, "r") \
        FOR_LINES(line, capacity, _file)

static
inline
DynamicString read_text_file(const char* file_path) {
    auto result = (DynamicString){};
    FOR_FILE(file, file_path, "r") {
        for (int ch; ch = fgetc(file), ch != EOF;) {
            APPEND(result, (char)ch);
        }
    }
    return result;
}
