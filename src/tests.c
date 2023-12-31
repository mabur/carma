#include <stdio.h>

#include "carma.h"

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
    if (a == b) { \
        printf("%s ok\n", (description)); \
    } else { \
        printf("%s %i!=%i bad\n", (description), (a), (b)); \
    } \
} while (0);

void test_find_if() {
    RANGE(int) range;
    INIT_RANGE(range, 4);
    range.first[0] = -1;
    range.first[1] = 4;
    range.first[2] = -2;
    range.first[3] = 1;
    ASSERT_EQUAL("FIND_IF", *FIND_IF(range, is_positive), 4);
    ASSERT_EQUAL("FIND_IF", *FIND_IF(range, is_negative), -1);
    ASSERT_EQUAL("FIND_IF", FIND_IF(range, is_zero), range.last);
    FREE_RANGE(range);
}

void test_find_if_backwards() {
    RANGE(int) range;
    INIT_RANGE(range, 4);
    range.first[0] = -1;
    range.first[1] = 4;
    range.first[2] = -2;
    range.first[3] = 1;
    ASSERT_EQUAL("FIND_IF_BACKWARDS", *FIND_IF_BACKWARDS(range, is_positive), 1);
    ASSERT_EQUAL("FIND_IF_BACKWARDS", *FIND_IF_BACKWARDS(range, is_negative), -2);
    ASSERT_EQUAL("FIND_IF_BACKWARDS", FIND_IF_BACKWARDS(range, is_zero), range.first - 1);
    FREE_RANGE(range);
}

void test_drop_while() {
    RANGE(int) range;
    INIT_RANGE(range, 4);
    range.first[0] = -1;
    range.first[1] = 4;
    range.first[2] = -2;
    range.first[3] = 1;
    ASSERT_EQUAL("DROP_WHILE", *DROP_WHILE(range, is_positive).first, -1);
    ASSERT_EQUAL("DROP_WHILE", *DROP_WHILE(range, is_negative).first, 4);
    ASSERT_EQUAL("DROP_WHILE", *DROP_WHILE(range, is_zero).first, -1);
    FREE_RANGE(range);
}

void test_drop_until() {
    RANGE(int) range;
    INIT_RANGE(range, 4);
    range.first[0] = -1;
    range.first[1] = 4;
    range.first[2] = -2;
    range.first[3] = 1;
    ASSERT_EQUAL("DROP_UNTIL", *DROP_UNTIL(range, is_positive).first, 4);
    ASSERT_EQUAL("DROP_UNTIL", *DROP_UNTIL(range, is_negative).first, -1);
    ASSERT_EQUAL("DROP_UNTIL", DROP_UNTIL(range, is_zero).first, range.last);
    FREE_RANGE(range);
}

void test_drop_back_while() {
    RANGE(int) range;
    INIT_RANGE(range, 4);
    range.first[0] = -1;
    range.first[1] = 4;
    range.first[2] = -2;
    range.first[3] = 1;
    ASSERT_EQUAL("DROP_BACK_WHILE", *(DROP_BACK_WHILE(range, is_positive).last - 1), -2);
    ASSERT_EQUAL("DROP_BACK_WHILE", *(DROP_BACK_WHILE(range, is_negative).last - 1), 1);
    ASSERT_EQUAL("DROP_BACK_WHILE", DROP_BACK_WHILE(range, is_zero).last, range.last);
    FREE_RANGE(range);
}

void test_drop_back_until() {
    RANGE(int) range;
    INIT_RANGE(range, 4);
    range.first[0] = -1;
    range.first[1] = 4;
    range.first[2] = -2;
    range.first[3] = 1;
    ASSERT_EQUAL("DROP_BACK_UNTIL", *(DROP_BACK_UNTIL(range, is_positive).last - 1), 1);
    ASSERT_EQUAL("DROP_BACK_UNTIL", *(DROP_BACK_UNTIL(range, is_negative).last - 1), -2);
    ASSERT_EQUAL("DROP_BACK_UNTIL", DROP_BACK_UNTIL(range, is_zero).last, range.first);
    FREE_RANGE(range);
}

void test_erase_if() {
    DARRAY(int) array;
    INIT_DARRAY(array, 4, 4);
    array.first[0] = -1;
    array.first[1] = 4;
    array.first[2] = -2;
    array.first[3] = 1;
    ERASE_IF(array, is_positive);
    ASSERT_EQUAL("ERASE_IF", COUNT(array), 2);
    ASSERT_EQUAL("ERASE_IF", *array.first, -1);
    ASSERT_EQUAL("ERASE_IF", *(array.last - 1), -2);
    FREE_DARRAY(array);
}

int main() {
    printf("TESTS BEGIN\n");
    test_find_if();
    test_find_if_backwards();
    test_drop_while();
    test_drop_until();
    test_drop_back_while();
    test_drop_back_until();
    printf("TESTS END\n");
    return 0;
}
