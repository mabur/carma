#include <ctype.h>
#include <stdio.h>
#include <carma/carma.h>
#include <carma/carma_string.h>
#include <carma/carma_table.h>

typedef struct {
    StringView keys;
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
    auto text = read_text_file(file_path);
    printf("%.*s\n", (int)text.count, text.data);
    
    auto text_view = (StringView){.data = text.data, .count=text.count};
    auto word = (StringView){};
    auto table = (Table){};
    auto total_words = 0;
    
    FOR_EACH_WORD(word, text_view, isspace) {
        total_words++;
        auto word_count = 0;
        GET_KEYS_VALUE(word, word_count, table);
        word_count++;
        SET_KEYS_VALUE(word, word_count, table);
    }

    auto total_unique_words = 0;
    
    FOR_EACH_TABLE(item, table) {
        total_unique_words++;
        printf("%.*s (%zu)\n", (int)item->keys.count, item->keys.data, item->value);
    }
    printf("\nTotal words %d\n", total_words);
    printf("Total unique words %d\n", total_unique_words);
    
    return EXIT_SUCCESS;
}
