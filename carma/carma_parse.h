#pragma once

#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "carma.h"
#include "carma_error.h"
#include "carma_make.h"
#include "carma_string.h"

/*
parse_int
parse_float
parse_double
parse_char
parse_line
parse_quoted_string
parse_word
parse_white_space
*/

typedef struct ParsedInt {
    int value;
    bool ok;
} ParsedInt;

typedef struct ParsedU64 {
    uint64_t value;
    bool ok;
} ParsedU64;

typedef struct ParsedDouble {
    double value;
    bool ok;
} ParsedDouble;

static inline
bool is_digit(char c) {
    return '0' <= c && c <= '9';
}

static inline
bool is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f';
}

static inline
ParsedU64 try_parse_u64(StringView* s) {
    auto result = MAKE(ParsedU64);
    while (!IS_EMPTY(*s) && is_digit(FIRST_ITEM(*s))) {
        result.value *= 10;
        result.value += FIRST_ITEM(*s) - '0';
        DROP_FRONT(*s);
        result.ok = true;
    }
    return result;
}

static inline
ParsedInt try_parse_int(StringView* s) {
    auto sign = +1;
    if (STARTS_WITH_ITEM(*s, '-')) {
        sign = -1;
        DROP_FRONT(*s);
    }
    auto abs_value = try_parse_u64(s);
    auto result = MAKE(ParsedInt);
    if (abs_value.ok) {
        result.value = sign * (int)abs_value.value;
        result.ok = true;
    }
    return result;
}

static inline
StringView parse_int_as_string(StringView* s) {
    auto input_data = s->data;
    auto input_count = s->count;
    if (IS_EMPTY(*s)) {
        return MAKE(StringView);
    }
    if (FIRST_ITEM(*s) == '+' || FIRST_ITEM(*s) == '-') {
        DROP_FRONT(*s);
    }
    if (IS_EMPTY(*s) || !is_digit(FIRST_ITEM(*s))) {
        return MAKE(StringView);
    }
    while (!IS_EMPTY(*s) && is_digit(FIRST_ITEM(*s))) {
        DROP_FRONT(*s);
    }
    return MAKE(StringView, .data=input_data, .count=input_count - s->count);
}

static inline
ParsedDouble try_parse_double(StringView* s) {
    auto sign = +1.0;
    if (STARTS_WITH_ITEM(*s, '-')) {
        sign = -1.0;
        DROP_FRONT(*s);
    }
    auto integral = 0.0;
    auto optional_integral = try_parse_u64(s);
    if (optional_integral.ok) {
        integral = (double)optional_integral.value;
    }
    auto fractional = 0.0;
    auto optional_fractional = MAKE(ParsedU64);
    if (STARTS_WITH_ITEM(*s, '.')) {
        DROP_FRONT(*s);
        auto count = s->count;
        optional_fractional = try_parse_u64(s);
        if (optional_fractional.ok) {
            fractional = (double)optional_fractional.value;
            for (; count > s->count; --count) {
                fractional *= 0.1;
            }
        }
    }
    auto result = MAKE(ParsedDouble);
    if (optional_integral.ok || optional_fractional.ok) {
        result.value = sign * (integral + fractional);
        result.ok = true;
    }
    return result;
}

static inline
StringView parse_quoted_string(StringView* s) {
    if (!STARTS_WITH_ITEM(*s, '"')) {
        return MAKE(StringView);
    }
    auto remaining = *s;
    DROP_FRONT(remaining); // Opening quote
    StringView value = {remaining.data, 0};
    while (!IS_EMPTY(remaining)) {
        if (FIRST_ITEM(remaining) == '"') {
            DROP_FRONT(remaining); // Closing quote
            *s = remaining;
            return value; // Success
        }
        else if (FIRST_ITEM(remaining) == '\\') {
            // Skip escape sequences
            DROP_FRONT(remaining);
            value.count++;
            if (IS_EMPTY(remaining)) {
                return value; // Dangling escape error
            }
            DROP_FRONT(remaining);
            value.count++;
        }
        else {
            DROP_FRONT(remaining);
            value.count++;
        }
    }
    // Unterminated string error
    return value;
}

static inline uint64_t parse_u64_or_exit(StringView* s) {
    auto optional = try_parse_u64(s);
    return GET_OPTIONAL_OR_EXIT(optional, "Could not parse uint64_t");
}

static inline int parse_int_or_exit(StringView* s) {
    auto before = *s;
    auto optional = try_parse_int(s);
    if (!optional.ok) {
        fprintf(stderr, "Could not parse int from string:\n");
        PRINT_CARMA_STRING(before);
    }
    return GET_OPTIONAL_OR_EXIT(optional, "Could not parse int");
}

static inline double parse_double_or_exit(StringView* s) {
    auto optional = try_parse_double(s);
    return GET_OPTIONAL_OR_EXIT(optional, "Could not parse double");
}

#define TRY_PARSE_U64(s) try_parse_u64(&(s))
#define TRY_PARSE_INT(s) try_parse_int(&(s))
#define TRY_PARSE_DOUBLE(s) try_parse_double(&(s))
#define PARSE_QUOTED_STRING(s) parse_quoted_string(&(s))

#define PARSE_U64(s) parse_u64_or_exit(&(s))
#define PARSE_INT(s) parse_int_or_exit(&(s))
#define PARSE_DOUBLE(s) parse_double_or_exit(&(s))

static inline
StringView parse_line(StringView* s) {
    StringView value = {s->data, 0};
    while (!IS_EMPTY(*s) && FIRST_ITEM(*s) != '\r' && FIRST_ITEM(*s) != '\n') {
        DROP_FRONT(*s);
        value.count++;
    }
    if (STARTS_WITH_ITEM(*s, '\r')) {
        DROP_FRONT(*s);
    }
    if (STARTS_WITH_ITEM(*s, '\n')) {
        DROP_FRONT(*s);
    }
    return value;
}

#define PARSE_LINE(s) parse_line(&(s))

static inline
void parse_whitespace(StringView* s) {
    while (!IS_EMPTY(*s) && is_whitespace(FIRST_ITEM(*s))) {
        DROP_FRONT(*s);
    }
}

#define PARSE_WHITESPACE(s) parse_whitespace(&(s))

static inline
bool parse_structural_character(StringView* s, char c) {
    parse_whitespace(s);
    if (!STARTS_WITH_ITEM(*s, c)) {
        return false;
    }
    DROP_FRONT(*s);
    parse_whitespace(s);
    return true;
}

#define PARSE_STRUCTURAL_CHARACTER(s, c) parse_structural_character(&(s), c)
