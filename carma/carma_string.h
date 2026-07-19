#pragma once

#include "carma_std.h"

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
#define STRING_LITERAL(cstring_literal) MAKE(StringView, (cstring_literal), sizeof(cstring_literal) - 1)

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
size_t carma_count_steps_until_delimiter(const char* begin, const char* end, char delimiter) {
    size_t steps = 0;
    for (const char* iterator = begin; iterator != end; ++iterator, ++steps) {
        if (*iterator == delimiter) {
            return steps;
        }
    }
    return steps;
}

static inline
size_t carma_count_steps_until(const char* begin, const char* end, int (*predicate)(int)) {
    size_t steps = 0;
    for (const char* iterator = begin; iterator != end; ++iterator, ++steps) {
        if (predicate(*iterator)) {
            return steps;
        }
    }
    return steps;
}

static inline
size_t carma_count_steps_while(const char* begin, const char* end, int (*predicate)(int)) {
    size_t steps = 0;
    for (const char* iterator = begin; iterator != end; ++iterator, ++steps) {
        if (!predicate(*iterator)) {
            return steps;
        }
    }
    return steps;
}


#define FOR_EACH_WORD(word, string, delimiter) \
    for (\
    StringView word = {(string).data, 0};\
    (word).count = carma_count_steps_until_delimiter((word).data, END_POINTER(string), (delimiter)),\
    (word).data != END_POINTER(string)\
    ;\
    (word).data += (END_POINTER(word) == END_POINTER(string) ? (word).count : (word).count + 1)\
    )

#define FOR_EACH_WORD_PREDICATE(word, string, is_delimiter) \
    for (\
    StringView word = {(string).data, 0};\
    (word).count = carma_count_steps_until((word).data, END_POINTER(string), (is_delimiter)),\
    (word).data != END_POINTER(string)\
    ;\
    (word).data += (word).count + carma_count_steps_while(END_POINTER(word), END_POINTER(string), (is_delimiter))\
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

static inline size_t carma_count_integer_digits(double x) {
    size_t digits = 1;
    for (double t = x; t >= 10.0; t /= 10.0) digits++;
    return digits;
}

static inline double carma_normalize_decimal(double x, size_t int_digits) {
    double scale = 1.0;
    for (size_t i = 0; i + 1 < int_digits; i++) scale *= 10.0;
    return x / scale;
}

#define SERIALIZE_DOUBLE(string_builder, x) do { \
    double _x = (double)(x); \
    if (_x != _x) { \
        SERIALIZE_CSTRING((string_builder), "nan"); \
    } else if (_x == 1.0 / 0.0) { \
        SERIALIZE_CSTRING((string_builder), "inf"); \
    } else if (_x == -1.0 / 0.0) { \
        SERIALIZE_CSTRING((string_builder), "-inf"); \
    } else if ((double)INTMAX_MIN <= _x && _x <= (double)INTMAX_MAX && _x == (double)(intmax_t)_x) { \
        SERIALIZE_INTEGRAL((string_builder), (intmax_t)_x); \
    } else { \
        if (_x < 0) { \
            APPEND((string_builder), '-'); \
            _x = -_x; \
        } \
        size_t _int_digits = carma_count_integer_digits(_x); \
        double _d = carma_normalize_decimal(_x, _int_digits); \
        for (size_t _i = 0; _i < _int_digits + 6; _i++) { \
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
