#pragma once

#include <limits.h>
#include <stddef.h>
#include <stdint.h>

#include "carma.h"
#include "carma_assert.h"
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

typedef struct OptionalInt {
    int data[1];
    size_t count;
} OptionalInt;

typedef struct OptionalU64 {
    uint64_t data[1];
    size_t count;
} OptionalU64;

typedef struct OptionalDouble {
    double data[1];
    size_t count;
} OptionalDouble;

static inline
bool is_digit(char c) {
    return '0' <= c && c <= '9';
}

static inline
bool is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f';
}

static inline
OptionalU64 parse_u64(StringView* s) {
    uint64_t parsed_value = 0;
    int has_parsed_digits = false;
    while (!IS_EMPTY(*s) && is_digit(FIRST_ITEM(*s))) {
        parsed_value *= 10;
        parsed_value += FIRST_ITEM(*s) - '0';
        DROP_FRONT(*s);
        has_parsed_digits = true;
    }
    if (has_parsed_digits) {
        return (OptionalU64){.data={parsed_value}, .count=1};
    }
    return (OptionalU64){};
}

static inline
OptionalInt parse_int(StringView* s) {
    auto sign = +1;
    if (STARTS_WITH_ITEM(*s, '-')) {
        sign = -1;
        DROP_FRONT(*s);
    }
    auto abs_value = parse_u64(s);
    FOR_EACH(it, abs_value) {
        return (OptionalInt){.data={sign * (int)(*it)}, .count=1};
    }
    return (OptionalInt){};
}

static inline
StringView parse_int_as_string(StringView* s) {
    auto input_data = s->data;
    auto input_count = s->count;
    if (IS_EMPTY(*s)) {
        return (StringView){};
    }
    if (FIRST_ITEM(*s) == '+' || FIRST_ITEM(*s) == '-') {
        DROP_FRONT(*s);
    }
    if (IS_EMPTY(*s) || !is_digit(FIRST_ITEM(*s))) {
        return (StringView){};
    }
    while (!IS_EMPTY(*s) && is_digit(FIRST_ITEM(*s))) {
        DROP_FRONT(*s);
    }
    return (StringView){.data=input_data, .count=input_count - s->count};
}

static inline OptionalDouble parse_double(StringView* s) {
    auto sign = +1.0;
    if (STARTS_WITH_ITEM(*s, '-')) {
        sign = -1.0;
        DROP_FRONT(*s);
    }
    auto integral = 0.0;
    auto optional_integral = parse_u64(s);
    FOR_EACH(it, optional_integral) {
        integral = (double)(*it);
    }
    auto fractional = 0.0;
    auto optional_fractional = (OptionalU64){};
    if (STARTS_WITH_ITEM(*s, '.')) {
        DROP_FRONT(*s);
        auto count = s->count;
        optional_fractional = parse_u64(s);
        FOR_EACH(it, optional_fractional) {
            fractional = (double)(*it);
            for (; count > s->count; --count) {
                fractional *= 0.1;
            }
        }
    }
    if (!IS_EMPTY(optional_integral) || !IS_EMPTY(optional_fractional)) {
        return (OptionalDouble){.data={sign * (integral + fractional)}, .count=1};
    }
    return (OptionalDouble){};
}

static inline uint64_t parse_u64_or_exit(StringView* s) {
    auto optional = parse_u64(s);
    CHECK_EXTERNAL(!IS_EMPTY(optional), "Could not parse uint64_t");
    return GET_OPTIONAL(optional);
}

static inline int parse_int_or_exit(StringView* s) {
    auto optional = parse_int(s);
    CHECK_EXTERNAL(!IS_EMPTY(optional), "Could not parse int");
    return GET_OPTIONAL(optional);
}

#define PARSE_U64(s) parse_u64(&(s))
#define PARSE_INT(s) parse_int(&(s))

#define PARSE_U64_OR_EXIT(s) parse_u64_or_exit(&(s))
#define PARSE_INT_OR_EXIT(s) parse_int_or_exit(&(s))
#define PARSE_DOUBLE(s) parse_double(&(s))

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
StringView parse_quoted_string(StringView* s) {
    if (!STARTS_WITH_ITEM(*s, '"')) {
        return (StringView){};
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

#define PARSE_QUOTED_STRING(s) parse_quoted_string(&(s))

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
