#include <math.h>
#include <stdio.h>
#include "carma.h"

typedef struct {
    int* data;
    int size;
    int capacity;
} Ints;

int main(int argc, char **argv) {
    let file_path = argv[1];
    printf(file_path);
    let file = fopen(file_path, "r");
    char buffer[256];
    
    Ints current_calories;
    Ints all_calories;
    INIT_DARRAY(current_calories, 0, 10000);
    INIT_DARRAY(all_calories, 0, 10000);
    
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
        let n = atoi(buffer);
        if (n == 0) {
            APPEND(all_calories, SUM_RANGE(current_calories));
            current_calories.size = 0;
        } else {
            APPEND(current_calories, n);
        }
    }
    fclose(file);
    
    let it = MAX_ELEMENT_RANGE(all_calories);
    
    printf("Max sum: %d", *it);

    FREE_RANGE(current_calories);
    FREE_RANGE(all_calories);
    
    return 0;
}
