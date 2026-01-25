#pragma once

#include <stddef.h>

#include "carma.h"
#include "carma_string.h"

/*

* Use Generic to overload a single serialization function?
  - bool/true/false/int gotchas.
* serialize vs format
* Have OpenGL like state machine, that checks grammar and manages indentation?

These concat to s:
serialize_indentation(&s, 4);
serialize_newline(&s, x);
serialize_string(&s, x);
serialize_char(&s, x);
serialize_int(&s, x);
serialize_double(&s, x);
serialize_bool(&s, x);
serialize_quoted_string(&s, x);
serialize_json_list_begin(&s);
serialize_json_list_end(&s);
serialize_json_object_begin(&s);
serialize_json_object_key(&s, "key");
serialize_json_object_end(&s);

auto s = (StringView){};
begin_json_object(&s);
    add_json_key(&s, "visible");
    add_json_bool(&s, true);
    add_json_key(&s, "width");
    add_json_int(&s, 640);
    add_json_key(&s, "height");
    add_json_int(&s, 480);
    add_json_key(&s, "pixel");
    begin_json_list(&s);
        add_json_int(&s, 0xFF00FF);
    end_json_list(&s);
end_json_object(&s);

auto j = (JsonBuilder){};
ADD_JSON_OBJECT(j) {
    ADD_JSON_KEY(j, "visible");
    ADD_JSON_BOOL(j, true);
    ADD_JSON_KEY(j, "width");
    ADD_JSON_INT(j, 640);
    ADD_JSON_KEY(j, "height");
    ADD_JSON_INT(j, 480);
    ADD_JSON_KEY(j, "pixels");
    ADD_JSON_LIST(j) {
        ADD_JSON_INT(j, 0xFF00FF);
    }
}

*/

typedef enum JsonContext {JSON_LIST, JSON_OBJECT} JsonContext;

typedef struct JsonContextStack {
    JsonContext* data;
    size_t count;
    size_t capacity;
} JsonContextStack;

typedef struct Json {
    StringBuilder string;
    JsonContextStack context_stack;
} Json;

static inline void carma_handle_json_list_delimiter(Json* json) {
    if (ENDS_WITH_ITEM(json->context_stack, JSON_LIST)) {
        if (!ENDS_WITH_ITEM(json->string, '[')) {
            APPEND(json->string, ',');
        }
    }
}

static inline void carma_handle_json_object_delimiter(Json* json) {
    if (ENDS_WITH_ITEM(json->context_stack, JSON_OBJECT)) {
        if (!ENDS_WITH_ITEM(json->string, '{')) {
            APPEND(json->string, ',');
        }
    }
}

static inline void carma_begin_json_list(Json* json) {
    carma_handle_json_list_delimiter(json);
    APPEND(json->string, '[');
    APPEND(json->context_stack, JSON_LIST);
}

static inline void carma_end_json_list(Json* json) {
    APPEND(json->string, ']');
    DROP_BACK(json->context_stack);
}

static inline void carma_begin_json_object(Json* json) {
    carma_handle_json_list_delimiter(json);
    APPEND(json->string, '{');
    APPEND(json->context_stack, JSON_OBJECT);
}

static inline void carma_end_json_object(Json* json) {
    APPEND(json->string, '}');
    DROP_BACK(json->context_stack);
}

#define ADD_JSON_INT(json, i) do { \
    carma_handle_json_list_delimiter(&json); \
    CONCAT_STRING((json).string, "%i", i); \
} while(0)

#define ADD_JSON_BOOL(json, b) do { \
    carma_handle_json_list_delimiter(&json); \
    CONCAT_STRING((json).string, "%s", b ? "true" : "false"); \
} while(0)

#define ADD_JSON_KEY(json, k) do { \
    carma_handle_json_object_delimiter(&json); \
    CONCAT_STRING((json).string, "\"%s\":", k); \
} while(0)

#define ADD_JSON_LIST(json) for ( \
    bool run = (carma_begin_json_list(&json), true); \
    run; \
    run=false, carma_end_json_list(&json))

#define ADD_JSON_OBJECT(json) for ( \
    bool run = (carma_begin_json_object(&json), true); \
    run; \
    run=false, carma_end_json_object(&json))
