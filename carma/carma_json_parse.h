#pragma once

#include "carma_parse.h"

/*
* Make parse_json_key_int_or_exit report outer file and line on failure.
* More principled distinction between empty collections and parsing failure for
  strings, arrays, objects.
*/

static inline
StringView parse_json_item(StringView* s);

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

static inline int parse_json_key_int_or_exit(StringView* s, const char* key) {
    auto value = parse_json_key(s, key);
    return PARSE_INT(value);
}

static inline double parse_json_key_double_or_exit(StringView* s, const char* key) {
    auto value = parse_json_key(s, key);
    return PARSE_DOUBLE(value);
}

#define PARSE_JSON_KEY_INT_OR_EXIT(s, key) parse_json_key_int_or_exit(&(s), key)
#define PARSE_JSON_KEY_DOUBLE_OR_EXIT(s, key) parse_json_key_double_or_exit(&(s), key)
