#include <stdio.h>
#include <carma/carma.h>

typedef struct {
    int* data;
    size_t count;
    size_t capacity;
} Ints;

int sum(Ints range) {
    auto result = 0;
    FOR_EACH(it, range) {
        result += *it;
    }
    return result;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        return EXIT_FAILURE;
    }
    auto file_path = argv[1];
    auto current_calories = (Ints){};
    auto all_calories = (Ints){};
    auto file = fopen(file_path, "r");
    FOR_LINES(line, 255, file) {
        auto n = atoi(line);
        if (n == 0) {
            APPEND(all_calories, sum(current_calories));
            CLEAR(current_calories);
        } else {
            APPEND(current_calories, n);
        }
    }
    fclose(file);
    FOR_MAX(it, all_calories) {
        printf("Max sum: %d", *it);
    }
    return EXIT_SUCCESS;
}
