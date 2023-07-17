#include <stdio.h>
#include "carma.h"

typedef struct {
    int* data;
    int size;
    int capacity;
} Ints;

int main(int argc, char **argv) {
    let file_path = argv[1];
    Ints current_calories;
    Ints all_calories;
    INIT_DARRAY(current_calories, 0, 10000);
    INIT_DARRAY(all_calories, 0, 10000);
    let file = fopen(file_path, "r");
    FOR_LINES(line, 255, file) {
        let n = atoi(line);
        if (n == 0) {
            APPEND(all_calories, SUM_RANGE(current_calories));
            CLEAR_RANGE(current_calories);
        } else {
            APPEND(current_calories, n);
        }
    }
    fclose(file);
    let it = MAX_ELEMENT_RANGE(all_calories);
    FREE_RANGE(current_calories);
    FREE_RANGE(all_calories);
    printf("Max sum: %d", *it);
    return 0;
}
