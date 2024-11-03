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
    auto whole = STRING_VIEW("hello small world");
    auto part = (StringView){};
    FOR_EACH_WORD(part, whole, isspace) {
        printf("%.*s\n", (int)part.count, part.data);
    }
    printf("END\n");
    
    auto text = read_text_file(file_path);
    auto text_view = (StringView){.data = text.data, .count=text.count};
    printf("%.*s\n", (int)text_view.count, text_view.data);
    printf("WORDS:\n");
    auto word_count = 0;
    FOR_EACH_WORD(part, text_view, isspace) {
        ++word_count;
        //printf("%.*s\n", (int)part.count, part.data);
        printf("Word length: %zu\n", part.count);
        FOR_EACH(c, part) {
            putchar(*c);
            
        }
        printf("\n");
    }
    printf("word_count=%d\n", word_count);
    
    return EXIT_SUCCESS;
}
