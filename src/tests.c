#include <stdio.h>

#include "carma.h"

typedef struct {
    int* first;
    int* last;
} Ints;

int is_positive(int x) {
    return x > 0;
}

#define ASSERT_EQUAL(description, a, b) do { \
    if (a == b) {                            \
        printf("%s ok\n", description);      \
    } else {                                 \
        printf("%s bad\n", description);     \
    }                                        \
} while (0);

void test_find_if() {
    Ints range;
    INIT_RANGE(range, 3);
    range.first[0] = -1;
    range.first[1] = 4;
    range.first[2] = -2;
    let it = FIND_IF(range, is_positive);
    ASSERT_EQUAL("FIND_IF", *it, 4);
    FREE_RANGE(range);
}

int main() {
    printf("TESTS BEGIN\n");
    test_find_if();
    printf("TESTS END\n");
    return 0;
}
