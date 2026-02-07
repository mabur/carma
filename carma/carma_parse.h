#pragma once

#include <limits.h>
#include <stddef.h>

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

Can wrap function calls in a macro to skip pointer syntax noise.

* Make parse_json_key_int_or_exit report outer file and line on failure.
* Have either both parse_int_advance(&s) and parse_int(s) functions, or convert to macro
*/

typedef struct OptionalInt {
    int data[1];
    int count;
} OptionalInt;

static inline
bool is_digit(char c) {
    return '0' <= c && c <= '9';
}

static inline
bool is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f';
}

static inline
StringView parse_json_item(StringView* s);

static inline
OptionalInt parse_int(StringView* s) {
    int parsed_value = 0;
    int sign = +1;
    int has_parsed_digits = false;
    if (IS_EMPTY(*s)) {
        return (OptionalInt){};
    }
    if (FIRST_ITEM(*s) == '-') {
        sign = -1;
        DROP_FRONT(*s);
    }
    else if (FIRST_ITEM(*s) == '+') {
        DROP_FRONT(*s);
    }
    while (!IS_EMPTY(*s) && is_digit(FIRST_ITEM(*s))) {
        parsed_value *= 10;
        parsed_value += FIRST_ITEM(*s) - '0';
        DROP_FRONT(*s);
        has_parsed_digits = true;
    }
    if (!has_parsed_digits) {
        return (OptionalInt){};
    }
    return (OptionalInt){.data={parsed_value * sign}, .count=1};
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

static inline
StringView parse_json_object_key(StringView* s) {
    parse_whitespace(s);
    auto key = parse_quoted_string(s);
    if (IS_EMPTY(key)) {
        return (StringView){};
    }
    if (!parse_structural_character(s, ':')) {
        return (StringView){};
    }
    return key;
}

static inline
StringView parse_json_object_value(StringView* s) {
    parse_whitespace(s);
    auto value = parse_json_item(s);
    if (IS_EMPTY(value)) {
        return (StringView){};
    }
    parse_whitespace(s);
    if (STARTS_WITH_ITEM(*s, ',')) {
        DROP_FRONT(*s);
    }
    else if (!STARTS_WITH_ITEM(*s, '}')) {
        return (StringView){};
    }
    return value;
}

static inline
StringView parse_first_json_array_item(StringView* s) {
    // In case of "[]" it advances the input past "[]", and returns an empty string.
    // In case of "[item]" it advances the input past "[item", and returns the item.
    // In case of "[item,]" it advances the input past "[item", and returns the item.
    if (!parse_structural_character(s, '[')) {
        return (StringView){};
    }
    if (parse_structural_character(s, ']')) {
        return (StringView){};
    }
    auto result = parse_json_item(s);
    if (IS_EMPTY(result)){
        return (StringView){};
    }
    return result;
}

static inline
StringView parse_next_json_array_item(StringView* s) {
    // In case of "]" it advances the input past "]", and returns an empty string.
    // In case of ",item]" it advances the input past ",item", and returns the item.
    // In case of ",item,]" it advances the input past the ",item", and returns the item.
    if (parse_structural_character(s, ']')) {
        return (StringView){};
    }
    if (!parse_structural_character(s, ',')) {
        return (StringView){};
    }
    auto result = parse_json_item(s);
    if (IS_EMPTY(result)){
        return (StringView){};
    }
    return result;
}

static inline
StringView parse_json_array(StringView* s) {
    StringView array = {s->data, 0};
    if (!parse_structural_character(s, '[')) {
        return (StringView){};
    }
    while (!IS_EMPTY(*s) && FIRST_ITEM(*s) != ']') {
        if (IS_EMPTY(parse_json_item(s))) {
            return (StringView){};
        }
        if (!parse_structural_character(s, ',')) {
            break;
        }
    }
    if (!parse_structural_character(s, ']')) {
        return (StringView){};
    }
    array.count = s->data - array.data;
    return array;
}

static inline
StringView parse_first_json_object_key(StringView* s) {
    // In case of {} it advances the input past {}, and returns an empty string.
    // In case of {"k":v} it advances the input past {"k" and returns k.
    if (!parse_structural_character(s, '{')) {
        return (StringView){};
    }
    if (parse_structural_character(s, '}')) {
        return (StringView){};
    }
    auto result = parse_quoted_string(s);
    if (IS_EMPTY(result)){
        return (StringView){};
    }
    return result;
}

static inline
StringView parse_next_json_object_value(StringView* s) {
    // In case of :v} it advances the input past :v and returns the value.
    StringView parsed_string = *s;
    if (!parse_structural_character(&parsed_string, ':')) {
        return (StringView){};
    }
    auto result = parse_json_item(&parsed_string);
    if (IS_EMPTY(result)){
        return (StringView){};
    }
    *s = parsed_string;
    return result;
}

static inline
StringView parse_next_json_object_key(StringView* s) {
    // In case of } it advances the input past } and returns an empty string.
    // In case of ,"k":v} it advances the input past ,"key" and returns the key.
    StringView parsed_string = *s;
    if (parse_structural_character(&parsed_string, '}')) {
        *s = parsed_string;
        return (StringView){};
    }
    if (!parse_structural_character(&parsed_string, ',')) {
        return (StringView){};
    }
    auto result = parse_quoted_string(&parsed_string);
    if (IS_EMPTY(result)){
        return (StringView){};
    }
    *s = parsed_string;
    return result;
}

static inline
StringView parse_json_object(StringView* s) {
    StringView parsed_string = *s;
    StringView object = {parsed_string.data, 0};
    if (!parse_structural_character(&parsed_string, '{')) {
        return (StringView){};
    }
    while (!parse_structural_character(&parsed_string, '}')) {
        if (IS_EMPTY(parse_json_object_key(&parsed_string))) {
            return (StringView){};
        }
        if (IS_EMPTY(parse_json_object_value(&parsed_string))) {
            return (StringView){};
        }
    }
    object.count = parsed_string.data - object.data;
    *s = parsed_string;
    return object;
}

static inline
StringView parse_json_item(StringView* s) {
    parse_whitespace(s);
    if (IS_EMPTY(*s)) {
        return (StringView){};
    }
    auto c = FIRST_ITEM(*s);
    switch (c) {
        case '{': return parse_json_object(s);
        case '[': return parse_json_array(s);
        case '"': return parse_quoted_string(s);
    }
    return parse_int_as_string(s);
}

#define FOR_EACH_JSON_ARRAY_ITEM(item, array) \
    for (auto (item) = parse_first_json_array_item(&array); \
    !IS_EMPTY(item); \
    (item) = parse_next_json_array_item(&array))

#define FOR_EACH_JSON_OBJECT_ITEM(key, value, object) \
    for ( \
        StringView (key) = parse_first_json_object_key(&object), \
        (value) = parse_next_json_object_value(&object); \
        !IS_EMPTY(key) && !IS_EMPTY(value); \
        (key) = parse_next_json_object_key(&object), \
        (value) = parse_next_json_object_value(&object) \
    )

static inline
StringView parse_json_key(StringView s, const char* key) {
    auto key2 = STRING_VIEW(key);
    FOR_EACH_JSON_OBJECT_ITEM(k, v, s) {
        if (ARE_EQUAL(key2, k)) {
            return v;
        }
    }
    return (StringView){};
}

static inline
int parse_json_key_int_or_exit(StringView s, const char* key) {
    auto value = parse_json_key(s, key);
    auto optional_int = parse_int(&value);
    CHECK_EXTERNAL(!IS_EMPTY(optional_int), "Could not parse int");
    return GET_OPTIONAL(optional_int);
}
