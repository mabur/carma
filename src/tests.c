#include <stdio.h>

#include "carma.h"

typedef struct {
    int* first;
    int* last;
} Ints;

int is_positive(int x) {
    return x > 0;
}

int is_negative(int x) {
    return x < 0;
}

int is_zero(int x) {
    return x == 0;
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
    INIT_RANGE(range, 4);
    range.first[0] = -1;
    range.first[1] = 4;
    range.first[2] = -2;
    range.first[2] = 1;
    ASSERT_EQUAL("FIND_IF", *FIND_IF(range, is_positive), 4);
    ASSERT_EQUAL("FIND_IF", *FIND_IF(range, is_negative), -1);
    ASSERT_EQUAL("FIND_IF", FIND_IF(range, is_zero), range.last);
    FREE_RANGE(range);
}

int main() {
    printf("TESTS BEGIN\n");
    test_find_if();
    printf("TESTS END\n");
    return 0;
}
