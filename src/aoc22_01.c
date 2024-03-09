#include <stdio.h>
#include "carma.h"

typedef struct {
    int* data;
    size_t count;
    size_t capacity;
} Ints;

int main(int argc, char **argv) {
    let file_path = argv[1];
    let current_calories = (Ints){};
    let all_calories = (Ints){};
    let file = fopen(file_path, "r");
    FOR_LINES(line, 255, file) {
        let n = atoi(line);
        if (n == 0) {
            APPEND(all_calories, SUM(current_calories));
            CLEAR(current_calories);
        } else {
            APPEND(current_calories, n);
        }
    }
    fclose(file);
    let it = MAX_ELEMENT(all_calories);
    printf("Max sum: %d", *it);
    return 0;
}
