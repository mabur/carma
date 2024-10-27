#include <ctype.h>
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


    printf("BEGIN\n");
    auto whole = CONSTANT_STRING("hello small world");
    auto part = (ConstantString){};
    FOR_EACH_WORD(part, whole, isspace) {
        printf("%.*s\n", (int)part.count, part.data);
    }
    printf("END\n");
    return EXIT_SUCCESS;
}
