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

typedef struct StringBuilder {
    char* data;
    size_t count;
    size_t capacity;
} StringBuilder;

#ifdef __cplusplus
#define STRING_VIEW(cstring) StringView{(cstring), strlen(cstring)}
#else
#define STRING_VIEW(cstring) (StringView){(cstring), strlen(cstring)}
#endif

#define CONCAT_CSTRING(string_builder, cstring) do { \
    StringView tail_with_null_terminator = {(cstring), strlen(cstring) + 1}; \
    CONCAT((string_builder), tail_with_null_terminator); \
    DROP_BACK(string_builder); \
} while (0)

static inline
StringBuilder carma_concat_string(StringBuilder string, const char* format, ...) {
    va_list args0;
    va_list args1;
    va_start(args0, format);
    va_copy(args1, args0);

    // Try writing at the end of the string:
    int num_characters = vsnprintf(
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

#define CONCAT_STRING(string_builder, format, ...) do { \
    (string_builder) = carma_concat_string((string_builder), (format), ##__VA_ARGS__); \
} while (0)

static inline
StringView FORMAT_STRING(const char* format, ...) {
    static StringBuilder string = {};
    CLEAR(string);
    va_list args;
    va_start(args, format);
    string = carma_concat_string(string, format, args);
    va_end(args);
    StringView result = {string.data, string.count};
    return result;
}

static inline
size_t carma_find_first_character_of(const char* data, size_t max_index, int (*predicate)(int)) {
    for (size_t i = 0; i < max_index; ++i) {
        if (predicate(data[i])) {
            return i;
        }
    }
    return max_index;
}

static inline
size_t carma_find_first_character_not_of(const char* data, size_t max_index, int (*predicate)(int)) {
    for (size_t i = 0; i < max_index; ++i) {
        if (!predicate(data[i])) {
            return i;
        }
    }
    return max_index;
}

#define FOR_EACH_WORD(word, string, is_delimiter) \
    StringView word = {}; \
    for (\
    size_t _word_length,\
    _delimeter_length\
    ;\
    _word_length = carma_find_first_character_of((string).data, (string).count, (is_delimiter)),\
    (word).data = (string).data,\
    (word).count = _word_length,\
    (string).data += _word_length,\
    (string).count -= _word_length,\
    !IS_EMPTY(word)\
    ;\
    _delimeter_length = carma_find_first_character_not_of((string).data, (string).count, (is_delimiter)),\
    (string).data += _delimeter_length,\
    (string).count -= _delimeter_length\
    )
    
////////////////////////////////////////////////////////////////////////////////
// FILE ALGORITHMS

#define FOR_FILE(file, filepath, mode) for (CARMA_AUTO file = fopen((file_path), mode); (file); fclose(file), (file) = 0)

#define FOR_LINES(line, capacity, file) for (char line[capacity]; fgets(line, (capacity), (file)) != NULL;)

#define READ_LINES(line, capacity, file_path) \
    FOR_FILE(_file, file_path, "r") \
        FOR_LINES(line, capacity, _file)

static inline
StringBuilder read_text_file(const char* file_path) {
    StringBuilder result = {};
    FOR_FILE(file, file_path, "r") {
        for (int ch; ch = fgetc(file), ch != EOF;) {
            APPEND(result, (char)ch);
        }
    }
    return result;
}
