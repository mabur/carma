#include <stdio.h>

#include <carma/carma.h>

typedef struct {
    int* data;
    size_t width;
    size_t height;
    size_t count;
} Image;

int is_positive(int x) {
    return x > 0;
}

int is_negative(int x) {
    return x < 0;
}

int is_zero(int x) {
    return x == 0;
}

int square(int x) {
    return x * x;
}

int global_assert_count = 0;
int global_assert_errors = 0;

#define ASSERT_EQUAL(description, a, b) do { \
    global_assert_count++; \
    if (a == b) { \
        printf("%s ok\n", (description)); \
    } else { \
        printf("%s %i!=%i bad\n", (description), (a), (b)); \
        global_assert_errors++; \
    } \
} while (0);

#define PRINT_RANGE(item_format, range) do { \
    printf("["); \
    FOR_EACH(it, (range)) { \
        printf(item_format, *it); \
    } \
    printf("]"); \
} while (0);

#define ASSERT_EQUAL_RANGE(description, a, b) do { \
    global_assert_count++; \
    if (EQUAL_RANGES(a, b)) { \
        printf("%s ok\n", (description)); \
    } else { \
        printf("%s ", (description)); \
        PRINT_RANGE("%i", a); \
        printf("!="); \
        PRINT_RANGE("%i", b); \
        printf(" bad\n"); \
        global_assert_errors++; \
    } \
} while (0);

void summarize_tests() {
    if (global_assert_errors != 0) {
        printf("%d/%d test failed\n", global_assert_errors, global_assert_count);
    } else {
        printf("All %d test succeeded\n", global_assert_count);
    }
}

void test_init_image() {
    Image image;
    INIT_IMAGE(image, 2, 3);
    ASSERT_EQUAL("test_init_image width", image.width, 2);
    ASSERT_EQUAL("test_init_image height", image.height, 3);
    ASSERT_EQUAL("test_init_image count", image.count, 2 * 3);
    FREE_IMAGE(image);
}

void test_for_each() {
    __auto_type actual = MAKE_DARRAY(int, 1, 2, 3);
    FOR_EACH(it, actual) {
        *it = square(*it);
    }
    __auto_type expected = MAKE_DARRAY(int, 1, 4, 9);
    ASSERT_EQUAL_RANGE("test_for_each", actual, expected);  
}

void test_for_each2() {
    __auto_type input = MAKE_DARRAY(int, 1, 2, 3);
    __auto_type actual = MAKE_DARRAY(int, 0, 0, 0);
    __auto_type expected = MAKE_DARRAY(int, 1, 4, 9);
    FOR_EACH2(a, b, actual, input) {
        *a = square(*b);
    }
    ASSERT_EQUAL_RANGE("test_for_each2", actual, expected);
}

void test_enumerate() {
    __auto_type actual = MAKE_DARRAY(int, 1, 2, 3);
    ENUMERATE(i, actual) {
        actual.data[i] = square(actual.data[i]);
    }
    __auto_type expected = MAKE_DARRAY(int, 1, 4, 9);
    ASSERT_EQUAL_RANGE("test_enumerate", actual, expected);
}

void test_fill() {
    __auto_type actual = MAKE_DARRAY(int, 1, 2, 3);
    FILL(actual, 3);
    __auto_type expected = MAKE_DARRAY(int, 3, 3, 3);
    ASSERT_EQUAL_RANGE("test_fill", actual, expected);
}

void test_find_if() {
    __auto_type range = MAKE_RANGE(int, -1, 4, -2, 1);
    ASSERT_EQUAL("FIND_IF", *FIND_IF(range, is_positive), 4);
    ASSERT_EQUAL("FIND_IF", *FIND_IF(range, is_negative), -1);
    ASSERT_EQUAL("FIND_IF", FIND_IF(range, is_zero), END_POINTER(range));
    FREE_RANGE(range);
}

void test_find_if_backwards() {
    __auto_type range = MAKE_RANGE(int, -1, 4, -2, 1);
    ASSERT_EQUAL("FIND_IF_BACKWARDS", *FIND_IF_BACKWARDS(range, is_positive), 1);
    ASSERT_EQUAL("FIND_IF_BACKWARDS", *FIND_IF_BACKWARDS(range, is_negative), -2);
    ASSERT_EQUAL("FIND_IF_BACKWARDS", FIND_IF_BACKWARDS(range, is_zero), range.data - 1);
    FREE_RANGE(range);
}

void test_drop_while() {
    __auto_type range = MAKE_RANGE(int, -1, 4, -2, 1);
    ASSERT_EQUAL("DROP_WHILE", *DROP_WHILE(range, is_positive).data, -1);
    ASSERT_EQUAL("DROP_WHILE", *DROP_WHILE(range, is_negative).data, 4);
    ASSERT_EQUAL("DROP_WHILE", *DROP_WHILE(range, is_zero).data, -1);
    FREE_RANGE(range);
}

void test_drop_until() {
    __auto_type range = MAKE_RANGE(int, -1, 4, -2, 1);
    ASSERT_EQUAL("DROP_UNTIL", *DROP_UNTIL(range, is_positive).data, 4);
    ASSERT_EQUAL("DROP_UNTIL", *DROP_UNTIL(range, is_negative).data, -1);
    ASSERT_EQUAL("DROP_UNTIL", DROP_UNTIL(range, is_zero).data, END_POINTER(range));
    FREE_RANGE(range);
}

void test_drop_back_while() {
    __auto_type range = MAKE_RANGE(int, -1, 4, -2, 1);
    ASSERT_EQUAL("DROP_BACK_WHILE", *(END_POINTER(DROP_BACK_WHILE(range, is_positive)) - 1), -2);
    ASSERT_EQUAL("DROP_BACK_WHILE", *(END_POINTER(DROP_BACK_WHILE(range, is_negative)) - 1), 1);
    ASSERT_EQUAL("DROP_BACK_WHILE", END_POINTER(DROP_BACK_WHILE(range, is_zero)), END_POINTER(range));
    FREE_RANGE(range);
}

void test_drop_back_until() {
    __auto_type range = MAKE_RANGE(int, -1, 4, -2, 1);
    ASSERT_EQUAL("DROP_BACK_UNTIL", *(END_POINTER(DROP_BACK_UNTIL(range, is_positive)) - 1), 1);
    ASSERT_EQUAL("DROP_BACK_UNTIL", *(END_POINTER(DROP_BACK_UNTIL(range, is_negative)) - 1), -2);
    ASSERT_EQUAL("DROP_BACK_UNTIL", END_POINTER(DROP_BACK_UNTIL(range, is_zero)), range.data);
    FREE_RANGE(range);
}

void test_erase_index1() {
    __auto_type actual = MAKE_DARRAY(int, 9);
    ERASE_INDEX(actual, 0);
    __auto_type expected = MAKE_DARRAY(int);
    ASSERT_EQUAL_RANGE("ERASE_INDEX 1", actual, expected);
}

void test_erase_index2a() {
    __auto_type actual = MAKE_DARRAY(int, 9, 8);
    ERASE_INDEX(actual, 0);
    __auto_type expected = MAKE_DARRAY(int, 8);
    ASSERT_EQUAL_RANGE("ERASE_INDEX 2a", actual, expected);
}

void test_erase_index2b() {
    __auto_type actual = MAKE_DARRAY(int, 9, 8);
    ERASE_INDEX(actual, 1);
    __auto_type expected = MAKE_DARRAY(int, 9);
    ASSERT_EQUAL_RANGE("ERASE_INDEX 2b", actual, expected);
}

void test_erase_index3a() {
    __auto_type actual = MAKE_DARRAY(int, 9, 8, 7);
    ERASE_INDEX(actual, 0);
    __auto_type expected = MAKE_DARRAY(int, 7, 8);
    ASSERT_EQUAL_RANGE("ERASE_INDEX 3a", actual, expected);
}

void test_erase_index3b() {
    __auto_type actual = MAKE_DARRAY(int, 9, 8, 7);
    ERASE_INDEX(actual, 1);
    __auto_type expected = MAKE_DARRAY(int, 9, 7);
    ASSERT_EQUAL_RANGE("ERASE_INDEX 3b", actual, expected);
}

void test_erase_index3c() {
    __auto_type actual = MAKE_DARRAY(int, 9, 8, 7);
    ERASE_INDEX(actual, 2);
    __auto_type expected = MAKE_DARRAY(int, 9, 8);
    ASSERT_EQUAL_RANGE("ERASE_INDEX 3b", actual, expected);
}

void test_erase_if_unallocated() {
    DARRAY(int) array;
    array.data = 0;
    array.count = 0;
    array.capacity = 0;

    ERASE_IF(array, is_zero);
    ASSERT_EQUAL("ERASE_IF EMPTY", array.count, 0);
    ASSERT_EQUAL_RANGE("ERASE_IF EMPTY", array, array);
    FREE_DARRAY(array);
}

void test_erase_if_empty() {
    __auto_type array = MAKE_DARRAY(int);
    ERASE_IF(array, is_zero);
    ASSERT_EQUAL("ERASE_IF EMPTY", array.count, 0);
    ASSERT_EQUAL_RANGE("ERASE_IF EMPTY", array, array);
    FREE_DARRAY(array);
}

void test_erase_if0() {
    __auto_type actual = MAKE_DARRAY(int, 0);
    ERASE_IF(actual, is_zero);
    __auto_type expected = MAKE_DARRAY(int);
    ASSERT_EQUAL_RANGE("ERASE_IF 0", actual, expected);
}

void test_erase_if1() {
    __auto_type actual = MAKE_DARRAY(int, 1);
    ERASE_IF(actual, is_zero);
    __auto_type expected = MAKE_DARRAY(int, 1);
    ASSERT_EQUAL_RANGE("ERASE_IF 1", actual, expected);
}

void test_erase_if11() {
    __auto_type actual = MAKE_DARRAY(int, 1, 1);
    ERASE_IF(actual, is_zero);
    __auto_type expected = MAKE_DARRAY(int, 1, 1);
    ASSERT_EQUAL_RANGE("ERASE_IF 11", actual, expected);
}

void test_erase_if00() {
    __auto_type actual = MAKE_DARRAY(int, 0, 0);
    ERASE_IF(actual, is_zero);
    __auto_type expected = MAKE_DARRAY(int);
    ASSERT_EQUAL_RANGE("ERASE_IF 00", actual, expected);
}

void test_erase_if10() {
    __auto_type actual = MAKE_DARRAY(int, 1, 0);
    ERASE_IF(actual, is_zero);
    __auto_type expected = MAKE_DARRAY(int, 1);
    ASSERT_EQUAL_RANGE("ERASE_IF 10", actual, expected);
}

void test_erase_if01() {
    __auto_type actual = MAKE_DARRAY(int, 0, 1);
    ERASE_IF(actual, is_zero);
    __auto_type expected = MAKE_DARRAY(int, 1);
    ASSERT_EQUAL_RANGE("ERASE_IF 01", actual, expected);
}

void test_erase_if101() {
    __auto_type actual = MAKE_DARRAY(int, 1, 0, 1);
    ERASE_IF(actual, is_zero);
    __auto_type expected = MAKE_DARRAY(int, 1, 1);
    ASSERT_EQUAL_RANGE("ERASE_IF 101", actual, expected);
}

void test_erase_if010() {
    __auto_type actual = MAKE_DARRAY(int, 0, 1, 0);
    ERASE_IF(actual, is_zero);
    __auto_type expected = MAKE_DARRAY(int, 1);
    ASSERT_EQUAL_RANGE("ERASE_IF 010", actual, expected);
}

void test_erase_if() {
    __auto_type actual = MAKE_DARRAY(int, -1, 4, -2, 1);
    ERASE_IF(actual, is_positive);
    __auto_type expected = MAKE_DARRAY(int, -1, -2);
    ASSERT_EQUAL_RANGE("ERASE_IF", actual, expected);
}

void test_append() {
    DARRAY(int) array;
    INIT_DARRAY(array, 0, 0);
    ASSERT_EQUAL("APPEND", array.count, 0);
    ASSERT_EQUAL("APPEND", array.capacity, 0);
    APPEND(array, 1);
    ASSERT_EQUAL("APPEND", array.count, 1);
    ASSERT_EQUAL("APPEND", array.capacity, 1);
    APPEND(array, 2);
    ASSERT_EQUAL("APPEND", array.count, 2);
    ASSERT_EQUAL("APPEND", array.capacity, 2);
    APPEND(array, 3);
    ASSERT_EQUAL("APPEND", array.count, 3);
    ASSERT_EQUAL("APPEND", array.capacity, 4);
    APPEND(array, 4);
    ASSERT_EQUAL("APPEND", array.count, 4);
    ASSERT_EQUAL("APPEND", array.capacity, 4);
    APPEND(array, 5);
    ASSERT_EQUAL("APPEND", array.count, 5);
    ASSERT_EQUAL("APPEND", array.capacity, 8);
    FREE_DARRAY(array);
}

int main() {
    test_init_image();
    
    test_for_each();
    test_for_each2();
    test_enumerate();

    test_fill();
    
    test_find_if();
    test_find_if_backwards();
    test_drop_while();
    test_drop_until();
    test_drop_back_while();
    test_drop_back_until();

    test_erase_index1();
    test_erase_index2a();
    test_erase_index2b();
    test_erase_index3a();
    test_erase_index3b();
    test_erase_index3c();
    
    test_erase_if_unallocated();
    test_erase_if_empty();
    test_erase_if();
    test_erase_if0();
    test_erase_if1();
    test_erase_if00();
    test_erase_if11();
    test_erase_if01();
    test_erase_if10();
    test_erase_if101();
    test_erase_if010();
    
    test_append();
    summarize_tests();
    return 0;
}
