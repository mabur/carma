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

Return optional, for error handling?
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
void parse_json_item(StringView* s);

static inline
int parse_int(StringView* s) {
    StringView parsed_string = *s;
    int parsed_value = 0;
    int sign = +1;
    bool has_parsed_digits = false;
    if (IS_EMPTY(parsed_string)) {
        return 0;
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

        *s = parsed_string;
    }
    return parsed_value * sign;
}

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

static inline
void parse_whitespace(StringView* s) {
    while (!IS_EMPTY(*s) && is_whitespace(FIRST_ITEM(*s))) {
        DROP_FRONT(*s);
    }
}

static inline
StringView parse_quoted_string(StringView* s) {
    if (IS_EMPTY(*s) || FIRST_ITEM(*s) != '"') {
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

static inline
StringView parse_json_list(StringView* s) {
    StringView parsed_string = *s;
    parse_whitespace(&parsed_string);
    if (!STARTS_WITH_ITEM(parsed_string, '[')) {
        return (StringView){};
    }
    StringView list = {parsed_string.data, 0};
    DROP_FRONT(parsed_string);
    while (!IS_EMPTY(parsed_string) && FIRST_ITEM(parsed_string) != ']') {
        parse_json_item(&parsed_string);
        parse_whitespace(&parsed_string);
        if (STARTS_WITH_ITEM(parsed_string, ',')) {
            DROP_FRONT(parsed_string);
            parse_whitespace(&parsed_string);
        }
        else {
            break;
        }
    }
    if (!STARTS_WITH_ITEM(parsed_string, ']')) {
        return (StringView){};
    }
    DROP_FRONT(parsed_string);
    list.count = parsed_string.data - list.data;
    *s = parsed_string;
    return list;
}

static inline
void parse_json_item(StringView* s) {
    parse_whitespace(s);
    (void)parse_int(s);
    (void)parse_quoted_string(s);
    (void)parse_json_list(s);
}
