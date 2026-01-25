#pragma once

#include <stddef.h>

#include "carma.h"
#include "carma_string.h"

/*

* Use Generic to overload a single serialization function?
  - bool/true/false/int gotchas.
* serialize vs format
* Have OpenGL like state machine, that checks grammar and manages indendation?

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
serialize_json_object_begin(&s);
    serialize_json_object_key(&s, "visible");
    serialize_bool(&s, true);
    serialize_json_object_key(&s, "width");
    serialize_int(&s, 640);
    serialize_json_object_key(&s, "height");
    serialize_int(&s, 480);
    serialize_json_object_key(&s, "pixel");
    serialize_json_list_begin(&s);
        serialize_json_list_item(&s);
        serialize_int(&s, 0xFF00FF);
    serialize_json_list_end(&s);
serialize_json_object_end(&s);

auto j = (JsonBuilder){};
BEGIN_JSON_OBJECT(j);
    ADD_JSON_KEY(j, "visible");
    ADD_JSON_BOOL(j, true);
    ADD_JSON_KEY(j, "width");
    ADD_JSON_INT(j, 640);
    ADD_JSON_KEY(j, "height");
    ADD_JSON_INT(j, 480);
    ADD_JSON_KEY(j, "pixels");
    BEGIN_JSON_LIST(j);
        ADD_JSON_INT(j, 0xFF00FF);
    END_JSON_LIST(j);
END_JSON_OBJECT(j);

auto j = (JsonBuilder){};
BEGIN_JSON_OBJECT(j);
{
    ADD_JSON_KEY(j, "visible");
    ADD_JSON_BOOL(j, true);
    ADD_JSON_KEY(j, "width");
    ADD_JSON_INT(j, 640);
    ADD_JSON_KEY(j, "height");
    ADD_JSON_INT(j, 480);
    ADD_JSON_KEY(j, "pixels");
    BEGIN_JSON_LIST(j);
    {
        ADD_JSON_INT(j, 0xFF00FF);
    }
    END_JSON_LIST(j);
}
END_JSON_OBJECT(j);

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

#define ADD_JSON_INT(json, i) do { \
        if (ENDS_WITH_ITEM((json).context_stack, JSON_LIST)) { \
            if (!ENDS_WITH_ITEM((json).string, '[')) { \
                APPEND((json).string, ','); \
            } \
        } \
        CONCAT_STRING((json).string, "%i", i); \
    } while(0)

#define ADD_JSON_BOOL(json, b) do { \
        CONCAT_STRING((json).string, "%s", b ? "true" : "false"); \
    } while(0)

#define ADD_JSON_KEY(json, k) do { \
        if (ENDS_WITH_ITEM((json).context_stack, JSON_OBJECT)) { \
            if (!ENDS_WITH_ITEM((json).string, '{')) { \
                APPEND((json).string, ','); \
            } \
        } \
        CONCAT_STRING((json).string, "\"%s\":", k); \
    } while(0)

void carma_begin_json_list(Json* json) {
    APPEND(json->string, '[');
    APPEND(json->context_stack, JSON_LIST);
}

void carma_end_json_list(Json* json) {
    APPEND(json->string, ']');
    DROP_BACK(json->context_stack);
}

void carma_begin_json_object(Json* json) {
    APPEND(json->string, '{');
    APPEND(json->context_stack, JSON_OBJECT);
}

void carma_end_json_object(Json* json) {
    APPEND(json->string, '}');
    DROP_BACK(json->context_stack);
}

#define ADD_JSON_LIST(json) for ( \
    bool run = (carma_begin_json_list(&json), true); \
    run; \
    run=false, carma_end_json_list(&json))

#define ADD_JSON_OBJECT(json) for ( \
    bool run = (carma_begin_json_object(&json), true); \
    run; \
    run=false, carma_end_json_object(&json))
