#include <stdio.h>
#include <carma/carma.h>

typedef struct {
    int* data;
    size_t count;
    size_t capacity;
} Ints;

int main(int argc, char **argv) {
    auto file_path = argv[1];
    auto current_calories = (Ints){};
    auto all_calories = (Ints){};
    auto file = fopen(file_path, "r");
    FOR_LINES(line, 255, file) {
        auto n = atoi(line);
        if (n == 0) {
            APPEND(all_calories, SUM(current_calories));
            CLEAR(current_calories);
        } else {
            APPEND(current_calories, n);
        }
    }
    fclose(file);
    auto it = MAX_ELEMENT(all_calories);
    printf("Max sum: %d", *it);
    return 0;
}
