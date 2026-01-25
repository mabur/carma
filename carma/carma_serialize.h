#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "carma.h"
#include "carma_string.h"

/*
auto j = (JsonBuilder){};
ADD_JSON_OBJECT(j) {
    ADD_JSON_KEY(j, "visible");
    ADD_JSON_BOOL(j, true);
    ADD_JSON_KEY(j, "width");
    ADD_JSON_INT(j, 640);
    ADD_JSON_KEY(j, "height");
    ADD_JSON_INT(j, 480);
    ADD_JSON_KEY(j, "pixels");
    ADD_JSON_ARRAY(j) {
        ADD_JSON_INT(j, 255);
    }
    ADD_JSON_KEY(j, "circle");
    ADD_JSON_OBJECT(j) {
        ADD_JSON_KEY(j, "radius");
        ADD_JSON_DOUBLE(j, 2.0);
    }
}
*/

typedef enum JsonContext {JSON_ARRAY, JSON_OBJECT} JsonContext;

typedef struct JsonContextStack {
    JsonContext* data;
    size_t count;
    size_t capacity;
} JsonContextStack;

typedef struct JsonBuilder {
    StringBuilder string;
    JsonContextStack context_stack;
} JsonBuilder;

static inline void carma_handle_json_array_delimiter(JsonBuilder* json) {
    if (ENDS_WITH_ITEM(json->context_stack, JSON_ARRAY)) {
        if (!ENDS_WITH_ITEM(json->string, '[')) {
            APPEND(json->string, ',');
        }
    }
}

static inline void carma_handle_json_object_delimiter(JsonBuilder* json) {
    if (ENDS_WITH_ITEM(json->context_stack, JSON_OBJECT)) {
        if (!ENDS_WITH_ITEM(json->string, '{')) {
            APPEND(json->string, ',');
        }
    }
}

static inline void carma_begin_json_array(JsonBuilder* json) {
    carma_handle_json_array_delimiter(json);
    APPEND(json->string, '[');
    APPEND(json->context_stack, JSON_ARRAY);
}

static inline void carma_end_json_array(JsonBuilder* json) {
    APPEND(json->string, ']');
    DROP_BACK(json->context_stack);
}

static inline void carma_begin_json_object(JsonBuilder* json) {
    carma_handle_json_array_delimiter(json);
    APPEND(json->string, '{');
    APPEND(json->context_stack, JSON_OBJECT);
}

static inline void carma_end_json_object(JsonBuilder* json) {
    APPEND(json->string, '}');
    DROP_BACK(json->context_stack);
}

#define ADD_JSON_INT(json, i) do { \
    carma_handle_json_array_delimiter(&json); \
    CONCAT_STRING((json).string, "%i", i); \
} while(0)

#define ADD_JSON_DOUBLE(json, d) do { \
    carma_handle_json_array_delimiter(&json); \
    CONCAT_STRING((json).string, "%f", d); \
} while(0)

#define ADD_JSON_BOOL(json, b) do { \
    carma_handle_json_array_delimiter(&json); \
    CONCAT_STRING((json).string, "%s", b ? "true" : "false"); \
} while(0)

#define ADD_JSON_KEY(json, k) do { \
    carma_handle_json_object_delimiter(&json); \
    CONCAT_STRING((json).string, "\"%s\":", k); \
} while(0)

#define ADD_JSON_ARRAY(json) for ( \
    bool run = (carma_begin_json_array(&json), true); \
    run; \
    run=false, carma_end_json_array(&json))

#define ADD_JSON_OBJECT(json) for ( \
    bool run = (carma_begin_json_object(&json), true); \
    run; \
    run=false, carma_end_json_object(&json))
