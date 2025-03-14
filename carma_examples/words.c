#include <ctype.h>
#include <stdio.h>
#include <carma/carma.h>
#include <carma/carma_string.h>
#include <carma/carma_table.h>

typedef struct {
    StringView key;
    size_t value;
    bool occupied;
} Item;

typedef struct {
    Item* data;
    size_t count;
    size_t capacity;
} Table;

Table count_words(StringView text) {
    auto table = (Table){};
    FOR_EACH_WORD(word, text, isspace) {
        auto word_count = 0;
        GET_RANGE_KEY_VALUE(word, word_count, table);
        word_count++;
        SET_RANGE_KEY_VALUE(word, word_count, table);
    }
    return table;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        return EXIT_FAILURE;
    }
    auto file_path = argv[1];
    auto text = read_text_file(file_path);
    auto table = count_words((StringView){.data = text.data, .count=text.count});
    FOR_EACH_TABLE(item, table) {
        auto word = item->key;
        auto word_count = item->value;
        printf("%.*s (%zu)\n", (int)word.count, word.data, word_count);
    }
    printf("%zu unique words\n", table.count);
    return EXIT_SUCCESS;
}
