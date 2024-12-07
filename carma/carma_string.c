#include "carma_string.h"

StringBuilder carma_format_string(StringBuilder string, const char* format, ...) {
    va_list args0;
    va_list args1;
    va_start(args0, format);
    va_copy(args1, args0);

    // Try writing at the end of the string:
    auto num_characters = vsnprintf(
        END_POINTER(string),
        REMAINING_CAPACITY(string),
        format,
        args0
    );
    // Check if we succeed:
    if (num_characters >= 0) {
        size_t required_capacity = string.count + (size_t)num_characters + 1;
        // Check if we need to reallocate the string to fit:
        if (required_capacity > string.capacity) {
            RESERVE_EXPONENTIAL_GROWTH(string, required_capacity);
            // Write the string that should fit now:
            num_characters = vsnprintf(
                END_POINTER(string), REMAINING_CAPACITY(string), format, args1
            );
        }
        string.count += (size_t)num_characters;
    }

    va_end(args0);
    va_end(args1);
    return string;
}

size_t carma_find_first_character_of(const char* data, size_t max_index, int (*predicate)(int)) {
    for (size_t i = 0; i < max_index; ++i) {
        if (predicate(data[i])) {
            return i;
        }
    }
    return max_index;
}

size_t carma_find_first_character_not_of(const char* data, size_t max_index, int (*predicate)(int)) {
    for (size_t i = 0; i < max_index; ++i) {
        if (!predicate(data[i])) {
            return i;
        }
    }
    return max_index;
}

StringBuilder read_text_file(const char* file_path) {
    auto result = (StringBuilder){};
    FOR_FILE(file, file_path, "r") {
        for (int ch; ch = fgetc(file), ch != EOF;) {
            APPEND(result, (char)ch);
        }
    }
    return result;
}
