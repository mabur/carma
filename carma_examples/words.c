#include <stdio.h>
#include <carma/carma.h>
#include <carma/carma_string.h>
#include <carma/carma_table.h>

typedef struct {
    char* data;
    size_t count;
    size_t capacity;
} String;

typedef struct {
    String keys;
    size_t value;
    bool occupied;
} Item;

typedef struct {
    Item* data;
    size_t count;
    size_t capacity;
} Table;

bool is_white_space(char c) {
    return c == ' ';
}

int main(int argc, char **argv) {
    if (argc < 2) {
        return EXIT_FAILURE;
    }
    auto file_path = argv[1];
    auto count = 0;
    auto word = (String){};
    auto table = (Table){};
    READ_LINES(line, 255, file_path) {

        RESERVE(word, strlen(line));
        // Copy line to word.
        // Split line into word.
        // Increase count for word in table.

        auto word_count = 0;
        FIND_KEYS(word, value_it, table) {
            word_count = *value_it;
        }
        SET_KEYS_VALUE(word, word_count, table);
        
        count++;
    }
    printf("The file %s contains %d lines\n", file_path, count);
    
    
    auto whole = CONSTANT_STRING("hello small world");
    while (!IS_EMPTY(whole)) {
        // Initialize empty part:
        auto part = (ConstantString){.data = whole.data, .count=0};
        // Grow part and shrink whole, until we find white space:
        for (;END_POINTER(part) < END_POINTER(whole) &&
            !is_white_space( *END_POINTER(part));
        ) {
            part.count++;
            DROP_FRONT(whole);
        }
        // Remove white space from whole:
        DROP_FRONT_WHILE(whole, is_white_space);
        
        printf("%.*s\n", (int)part.count, part.data);
    }
    
    return EXIT_SUCCESS;
}
