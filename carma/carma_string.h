#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "carma.h"

typedef struct StringView {
    const char* data;
    size_t count;
} StringView;

typedef struct StringBuilder {
    char* data;
    size_t count;
    size_t capacity;
} StringBuilder;

#define STRING_VIEW(cstring) (StringView){(cstring), strlen(cstring)}

#define CONCAT_CSTRING(dynamic_string, cstring) do { \
    StringView tail_with_null_terminator = {(cstring), strlen(cstring) + 1}; \
    CONCAT((dynamic_string), tail_with_null_terminator); \
    DROP_BACK(dynamic_string); \
} while (0)

StringBuilder carma_format_string(StringBuilder string, const char* format, ...);

#define FORMAT_STRING(string, format, ...) do { \
    (string) = carma_format_string((string), (format), ##__VA_ARGS__); \
} while (0)

size_t carma_find_first_character_of(const char* data, size_t max_index, int (*predicate)(int));
size_t carma_find_first_character_not_of(const char* data, size_t max_index, int (*predicate)(int));

#define FOR_EACH_WORD(word, string, is_delimeter) \
    for (\
    size_t _word_length,\
    _delimeter_length\
    ;\
    _word_length = carma_find_first_character_of((string).data, (string).count, (is_delimeter)),\
    (word).data = (string).data,\
    (word).count = _word_length,\
    (string).data += _word_length,\
    (string).count -= _word_length,\
    !IS_EMPTY(word)\
    ;\
    _delimeter_length = carma_find_first_character_not_of((string).data, (string).count, (is_delimeter)),\
    (string).data += _delimeter_length,\
    (string).count -= _delimeter_length\
    )
    
////////////////////////////////////////////////////////////////////////////////
// FILE ALGORITHMS

#define FOR_FILE(file, filepath, mode) for (auto file = fopen((file_path), mode); (file); fclose(file), (file) = 0)

#define FOR_LINES(line, capacity, file) for (char line[capacity]; fgets(line, (capacity), (file)) != NULL;)

#define READ_LINES(line, capacity, file_path) \
    FOR_FILE(_file, file_path, "r") \
        FOR_LINES(line, capacity, _file)
        
StringBuilder read_text_file(const char* file_path);
