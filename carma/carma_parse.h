#pragma once

#include <limits.h>
#include <stddef.h>

#include "carma.h"
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

auto x = 0;
parse_int(&s, &x);

auto x = 0;
PARSE_INT(s, x);

auto x = parse_int(&s);

auto x = PARSE_INT(s);

auto result = PARSE_INT(s);
auto x = result.value;
s = result.string;
*/

static inline
bool is_digit(char c) {
    return '0' <= c && c <= '9';
}

static inline
bool is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f';
}

static inline
void parse_int(StringView* s, int* value) {
    StringView parsed_string = *s;
    int parsed_value = 0;
    int sign = +1;
    bool has_parsed_digits = false;
    if (IS_EMPTY(parsed_string)) {
        return;
    }
    if (FIRST_ITEM(parsed_string) == '-') {
        sign = -1;
        DROP_FRONT(parsed_string);
    }
    else if (FIRST_ITEM(parsed_string) == '+') {
        DROP_FRONT(parsed_string);
    }
    while (!IS_EMPTY(parsed_string) && is_digit(FIRST_ITEM(parsed_string))) {
        parsed_value *= 10;
        parsed_value += FIRST_ITEM(parsed_string) - '0';
        DROP_FRONT(parsed_string);
        has_parsed_digits = true;
    }
    if (has_parsed_digits) {
        *value = parsed_value * sign;
        *s = parsed_string;
    }
}

static inline
void parse_line(StringView* s, StringView* value) {
    value->data = s->data;
    value->count = 0;
    while (!IS_EMPTY(*s) && FIRST_ITEM(*s) != '\r' && FIRST_ITEM(*s) != '\n') {
        DROP_FRONT(*s);
        value->count++;
    }
    if (!IS_EMPTY(*s) && FIRST_ITEM(*s) == '\r') {
        DROP_FRONT(*s);
    }
    if (!IS_EMPTY(*s) && FIRST_ITEM(*s) == '\n') {
        DROP_FRONT(*s);
    }
}

static inline
void parse_whitespace(StringView* s) {
    while (!IS_EMPTY(*s) && is_whitespace(FIRST_ITEM(*s))) {
        DROP_FRONT(*s);
    }
}
