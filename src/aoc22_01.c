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
    
    Ints words;
    INIT_DARRAY(words, 0, 10000);
    int max_sum = 0;
    
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
        let n = atoi(buffer);
        if (n == 0) {
            let sum = SUM_RANGE(words);
            max_sum = fmax(max_sum, sum);
            words.size = 0;
        } else {
            APPEND(words, n);
        }
    }
    fclose(file);
    
    printf("Max sum: %d", max_sum);

    FREE_RANGE(words);
    
    return 0;
}
