#include <stdio.h>
#include "carma.h"

int main(int argc, char **argv) {
    let file_path = argv[1];
    DARRAY(int) current_calories;
    DARRAY(int) all_calories;
    INIT_DARRAY(current_calories, 0, 10000);
    INIT_DARRAY(all_calories, 0, 10000);
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
    FREE_DARRAY(current_calories);
    FREE_DARRAY(all_calories);
    printf("Max sum: %d", *it);
    return 0;
}
