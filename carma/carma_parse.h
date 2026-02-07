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

* Make parse_json_key_int_or_exit report outer file and line on failure.
* More principled distinction between empty collections and parsing failure for
  strings, arrays, objects.
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
StringView parse_json_item(StringView* s);

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

#define PARSE_U64(s) parse_u64(&(s))

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


#define PARSE_INT(s) parse_int(&(s))

static inline
int parse_int_or_exit(StringView* s) {
    auto optional_int = parse_int(s);
    CHECK_EXTERNAL(!IS_EMPTY(optional_int), "Could not parse int");
    return GET_OPTIONAL(optional_int);
}

#define PARSE_INT_OR_EXIT(s) parse_int_or_exit(&(s))

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
    auto has_any_digits = false;
    if (STARTS_WITH_ITEM(*s, '-')) {
        sign = -1.0;
        DROP_FRONT(*s);
    }
    auto int_part = parse_u64(s);
    auto int_value = 0.0;
    if (int_part.count) {
        int_value = (double)int_part.data[0];
        has_any_digits = true;
    }
    auto fraction_value = 0.0;
    if (STARTS_WITH_ITEM(*s, '.')) {
        DROP_FRONT(*s);
        StringView frac_start = *s;
        OptionalU64 frac_part = parse_u64(s);
        if (frac_part.count) {
            uint64_t frac_u = frac_part.data[0];
            int frac_digits = (int)(frac_start.count - s->count);
            double frac = (double)frac_u;
            while (frac_digits--) {
                frac *= 0.1;
            }
            fraction_value += frac;
            has_any_digits = true;
        }
    }
    if (has_any_digits) {
        return (OptionalDouble){.data={sign * (int_value + fraction_value)}, .count=1};
    }
    return (OptionalDouble){};
}


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

#define PARSE_JSON_ARRAY(s) parse_json_array(&(s))

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
    if (!parse_structural_character(s, ':')) {
        return (StringView){};
    }
    auto result = parse_json_item(s);
    if (IS_EMPTY(result)){
        return (StringView){};
    }
    return result;
}

static inline
StringView parse_next_json_object_key(StringView* s) {
    // In case of } it advances the input past } and returns an empty string.
    // In case of ,"k":v} it advances the input past ,"key" and returns the key.
    if (parse_structural_character(s, '}')) {
        return (StringView){};
    }
    if (!parse_structural_character(s, ',')) {
        return (StringView){};
    }
    auto result = parse_quoted_string(s);
    if (IS_EMPTY(result)){
        return (StringView){};
    }
    return result;
}

static inline
StringView parse_json_object(StringView* s) {
    StringView object = {s->data, 0};
    if (!parse_structural_character(s, '{')) {
        return (StringView){};
    }
    while (!parse_structural_character(s, '}')) {
        if (IS_EMPTY(parse_json_object_key(s))) {
            return (StringView){};
        }
        if (IS_EMPTY(parse_json_object_value(s))) {
            return (StringView){};
        }
    }
    object.count = s->data - object.data;
    return object;
}

#define PARSE_JSON_OBJECT(s) parse_json_object(&(s))

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
StringView parse_json_key(StringView* s, const char* key) {
    auto key2 = STRING_VIEW(key);
    FOR_EACH_JSON_OBJECT_ITEM(k, v, *s) {
        if (ARE_EQUAL(key2, k)) {
            return v;
        }
    }
    return (StringView){};
}

#define PARSE_JSON_KEY(s, key) parse_json_key(&(s), (key))

static inline
int parse_json_key_int_or_exit(StringView* s, const char* key) {
    auto value = parse_json_key(s, key);
    return PARSE_INT_OR_EXIT(value);
}

#define PARSE_JSON_KEY_INT_OR_EXIT(s, key) parse_json_key_int_or_exit(&(s), key)
