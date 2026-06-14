#pragma once

#include <math.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "carma.h"
#include "carma_make.h"

typedef struct StringView {
    const char* data;
    size_t count;
} StringView;

typedef struct StringBuilder {
    char* data;
    size_t count;
    size_t capacity;
} StringBuilder;


////////////////////////////////////////////////////////////////////////////////
// GENERAL STRING MACROS

static inline char* carma_make_cstring(const char* data, size_t count) {
    char* result = (char*)malloc(count + 1);
    if (result != NULL) {
        memcpy(result, data, count);
        result[count] = '\0';
    }
    return result;
}

#define MAKE_CSTRING(string) carma_make_cstring((string).data, (string).count)

#define PRINT_CARMA_STRING(string) do { \
    FOR_EACH(c, (string)) { \
        putchar(*c); \
    } \
} while (0)

////////////////////////////////////////////////////////////////////////////////
// STRING VIEW MACROS

#define STRING_VIEW(cstring) MAKE(StringView, (cstring), strlen(cstring))

static inline
StringBuilder carma_vconcat_string(StringBuilder string, const char* format, va_list args) {
    va_list args1;
    va_copy(args1, args);

    // Try writing at the end of the string:
    int num_characters = vsnprintf(
        END_POINTER(string),
        REMAINING_CAPACITY(string),
        format,
        args
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

    va_end(args1);
    return string;
}

static inline
StringView FORMAT_STRING(const char* format, ...) {
    static StringBuilder string = {};
    CLEAR(string);
    va_list args;
    va_start(args, format);
    string = carma_vconcat_string(string, format, args);
    va_end(args);
    StringView result = {string.data, string.count};
    return result;
}

static inline
size_t carma_find_character(const char* begin, const char* end, char c) {
    size_t steps = 0;
    for (const char* iterator = begin; iterator != end; ++iterator, ++steps) {
        if (*iterator == c) {
            return steps;
        }
    }
    return steps;
}

static inline
size_t carma_find_character_if(const char* data, size_t max_index, int (*predicate)(int)) {
    for (size_t i = 0; i < max_index; ++i) {
        if (predicate(data[i])) {
            return i;
        }
    }
    return max_index;
}

static inline
size_t carma_find_character_not_if(const char* data, size_t max_index, int (*predicate)(int)) {
    for (size_t i = 0; i < max_index; ++i) {
        if (!predicate(data[i])) {
            return i;
        }
    }
    return max_index;
}


#define FOR_EACH_WORD(word, string, delimiter) \
    for (\
    StringView word = {(string).data};\
    (word).count = carma_find_character((word).data, END_POINTER(string), (delimiter)),\
    !IS_EMPTY(word)\
    ;\
    (word).data += (word).count + 1\
    )

#define FOR_EACH_WORD_PREDICATE(word, string, is_delimiter) \
    StringView word = {}; \
    for (\
    size_t _word_length,\
    _delimeter_length\
    ;\
    _word_length = carma_find_character_if((string).data, (string).count, (is_delimiter)),\
    (word).data = (string).data,\
    (word).count = _word_length,\
    (string).data += _word_length,\
    (string).count -= _word_length,\
    !IS_EMPTY(word)\
    ;\
    _delimeter_length = carma_find_character_not_if((string).data, (string).count, (is_delimiter)),\
    (string).data += _delimeter_length,\
    (string).count -= _delimeter_length\
    )

////////////////////////////////////////////////////////////////////////////////
// STRING BUILDER MACROS

static inline char* carma_as_cstring(StringBuilder* string_builder) {
    APPEND(*string_builder, '\0');
    DROP_BACK(*string_builder);
    return string_builder->data;
}

#define AS_CSTRING(string_builder) carma_as_cstring(&(string_builder))

#define SERIALIZE_INTEGRAL(string_builder, x) do { \
    CARMA_AUTO _si_x = (x); \
    if (_si_x < 0) { \
        _si_x = -_si_x; \
        APPEND((string_builder), '-'); \
    } \
    CARMA_AUTO _front = (string_builder).count; \
    do { \
        APPEND((string_builder), (char)('0' + _si_x % 10)); \
        _si_x /= 10; \
    } while (_si_x > 0); \
    for (CARMA_AUTO _si_back = (string_builder).count - 1; _front < _si_back; _front++, _si_back--) { \
        SWAP((string_builder).data[_front], (string_builder).data[_si_back]); \
    } \
    APPEND(string_builder, '\0'); \
    DROP_BACK(string_builder); \
} while(0)

#define SERIALIZE_DOUBLE(string_builder, x) do { \
    double _x = (double)(x); \
    if (isnan(_x)) { \
        SERIALIZE_CSTRING((string_builder), "nan"); \
    } else if (isinf(_x)) { \
        if (_x > 0) \
            SERIALIZE_CSTRING((string_builder), "inf"); \
        else \
            SERIALIZE_CSTRING((string_builder), "-inf"); \
    } else if (_x == floor(_x) && (double)INTMAX_MIN <= _x && _x <= (double)INTMAX_MAX) { \
        SERIALIZE_INTEGRAL((string_builder), (intmax_t)_x); \
    } else { \
        if (_x < 0) { \
            APPEND((string_builder), '-'); \
            _x = -_x; \
        } \
        int _int_digits = _x >= 1.0 ? (int)log10(_x) + 1 : 1; \
        double _d = _x / pow(10.0, _int_digits - 1); \
        for (int _i = 0; _i < _int_digits + 6; _i++) { \
            if (_i == _int_digits) APPEND((string_builder), '.'); \
            int _digit = (int)_d; \
            APPEND((string_builder), '0' + _digit); \
            _d = (_d - _digit) * 10.0; \
        } \
    } \
    APPEND(string_builder, '\0'); \
    DROP_BACK(string_builder); \
} while(0)

#define SERIALIZE_BOOL(string_builder, x) do { \
    if (x) SERIALIZE_CSTRING(string_builder, "true"); \
    else SERIALIZE_CSTRING(string_builder, "false"); \
} while(0)

#define SERIALIZE_CHARACTER(string_builder, x) do { \
    APPEND((string_builder), (x)); \
    APPEND((string_builder), '\0'); \
    DROP_BACK(string_builder); \
} while(0)

#define SERIALIZE_CSTRING(string_builder, cstring) do { \
    StringView tail_with_null_terminator = {(cstring), strlen(cstring) + 1}; \
    CONCAT((string_builder), tail_with_null_terminator); \
    DROP_BACK(string_builder); \
} while (0)

////////////////////////////////////////////////////////////////////////////////
// FILE ALGORITHMS

#define FOR_FILE(file, file_path, mode) for (CARMA_AUTO file = fopen((file_path), mode); (file); fclose(file), (file) = 0)

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
