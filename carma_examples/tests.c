#include <stdbool.h>
#include <stdio.h>

#include <carma/carma.h>
#include <carma/carma_string.h>
#include <carma/carma_table.h>

typedef struct {
    int* data;
    size_t count;
} IntRange;

typedef struct {
    int* data;
    size_t count;
    size_t capacity;
} IntArray;

typedef struct {
    int key;
    int value;
    bool occupied;
} ItemIntInt;

typedef struct {
    IntArray key;
    int value;
    bool occupied;
} ItemIntArrayInt;

typedef struct {
    ItemIntInt* data;
    size_t count;
    size_t capacity;
} TableIntInt;

typedef struct {
    ItemIntArrayInt* data;
    size_t count;
    size_t capacity;
} TableIntArrayInt;

#define VALUE_TYPE2(range_type) typeof(*((range_type){}).data)
#define MAKE_ARRAY_LITERAL(range_type, ...) (VALUE_TYPE2(range_type)[]){__VA_ARGS__}
#define COUNT_VARGS(range_type, ...) sizeof((VALUE_TYPE2(range_type)[]){__VA_ARGS__}) / ITEM_SIZE(range_type)

#define COPY_VARARGS_RAW(range_type, ...) memcpy(\
    malloc(sizeof(MAKE_ARRAY_LITERAL(range_type, __VA_ARGS__))),\
    MAKE_ARRAY_LITERAL(range_type, __VA_ARGS__),\
    sizeof(MAKE_ARRAY_LITERAL(range_type, __VA_ARGS__))\
)

#define MAKE_RANGE(range_type, ...) (range_type){\
    .data=COPY_VARARGS_RAW(range_type, __VA_ARGS__),\
    .count=COUNT_VARGS(range_type, __VA_ARGS__)\
}

#define MAKE_DARRAY(darray_type, ...) (darray_type){\
    .data=COPY_VARARGS_RAW(darray_type, __VA_ARGS__),\
    .count=COUNT_VARGS(darray_type, __VA_ARGS__),\
    .capacity=COUNT_VARGS(darray_type, __VA_ARGS__)\
}

typedef struct {
    int* data;
    size_t width;
    size_t height;
    size_t count;
} Image;

typedef struct {
    int* data;
    size_t width;
    size_t height;
    size_t depth;
    size_t count;
} Voxels;

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

int add(int x, int y) {
    return x + y;
}

int global_assert_count = 0;
int global_assert_errors = 0;

void ASSERT_BOOL(const char* description, bool a) {
    global_assert_count++;
    if (a) {
        printf("%s ok\n", (description));
    } else {
        printf("%s bad\n", (description));
        global_assert_errors++;
    }
}

void ASSERT_EQUAL_INT(const char* description, int a, int b) {
    global_assert_count++;
    if (a == b) {
        printf("%s ok\n", (description));
    } else {
        printf("%s %i!=%i bad\n", (description), (a), (b));
        global_assert_errors++;
    }
}

void ASSERT_EQUAL_POINTER(const char* description, const void* a, const void* b) {
    global_assert_count++;
    if (a == b) {
        printf("%s ok\n", (description));
    } else {
        printf("%s %p!=%p bad\n", (description), (a), (b));
        global_assert_errors++;
    }
}

void ASSERT_NOT_EQUAL_POINTER(const char* description, const void* a, const void* b) {
    global_assert_count++;
    if (a != b) {
        printf("%s ok\n", (description));
    } else {
        printf("%s %p==%p bad\n", (description), (a), (b));
        global_assert_errors++;
    }
}

void ASSERT_EQUAL_SIZE(const char* description, size_t a, size_t b) {
    global_assert_count++;
    if (a == b) {
        printf("%s ok\n", (description));
    } else {
        printf("%s %zu!=%zu bad\n", (description), (a), (b));
        global_assert_errors++;
    }
}

#define ASSERT_NOT_EQUAL_INT(description, a, b) do { \
    global_assert_count++; \
    if (a != b) { \
        printf("%s ok\n", (description)); \
    } else { \
        printf("%s %i==%i bad\n", (description), (a), (b)); \
        global_assert_errors++; \
    } \
} while (0)

#define PRINT_RANGE(item_format, range) do { \
    printf("["); \
    FOR_EACH(it, (range)) { \
        printf(item_format, *it); \
    } \
    printf("]"); \
} while (0)

#define ASSERT_EQUAL_RANGE(description, left_range, right_range) do { \
    global_assert_count++; \
    if (ARE_EQUAL(left_range, right_range)) { \
        printf("%s ok\n", (description)); \
    } else { \
        printf("%s ", (description)); \
        PRINT_RANGE("%i ", left_range); \
        printf(" != "); \
        PRINT_RANGE("%i ", right_range); \
        printf(" bad\n"); \
        global_assert_errors++; \
    } \
} while (0)

#define ASSERT_EQUAL_STRINGS(description, a, b) do { \
    global_assert_count++; \
    if (strcmp(a, b) == 0) { \
        printf("%s ok\n", (description)); \
    } else { \
        printf("%s ", (description)); \
        printf("%s", a); \
        printf("!="); \
        printf("%s", b); \
        printf(" bad\n"); \
        global_assert_errors++; \
    } \
} while (0)

void ASSERT_STRING_BUILDER(
    const char* description,
    StringBuilder string,
    const char* data,
    size_t count,
    size_t capacity
) {
    ASSERT_EQUAL_STRINGS((description), (string).data, (data));
    ASSERT_EQUAL_SIZE((description), (string).count, (count));
    ASSERT_EQUAL_SIZE((description), (string).capacity, (capacity));
}

void summarize_tests() {
    if (global_assert_errors != 0) {
        printf("%d/%d test failed\n", global_assert_errors, global_assert_count);
    } else {
        printf("All %d test succeeded\n", global_assert_count);
    }
}

void test_2d_array() {
    Image image;
    INIT_2D_ARRAY(image, 2, 3);
    ASSERT_EQUAL_SIZE("test_2d_array width", image.width, 2);
    ASSERT_EQUAL_SIZE("test_2d_array height", image.height, 3);
    ASSERT_EQUAL_SIZE("test_2d_array count", image.count, 2 * 3);
    FREE_2D_ARRAY(image);
}

void test_3d_array() {
    Voxels voxels;
    INIT_3D_ARRAY(voxels, 2, 3, 4);
    ASSERT_EQUAL_SIZE("test_3d_array width", voxels.width, 2);
    ASSERT_EQUAL_SIZE("test_3d_array height", voxels.height, 3);
    ASSERT_EQUAL_SIZE("test_3d_array depth", voxels.depth, 4);
    ASSERT_EQUAL_SIZE("test_3d_array count", voxels.count, 2 * 3 * 4);
    FREE_3D_ARRAY(voxels);
}

void test_is_empty() {
    auto actual = (IntArray){};
    ASSERT_EQUAL_INT("test_is_empty", IS_EMPTY(actual), true);
    APPEND(actual, 1);
    ASSERT_EQUAL_INT("test_is_empty", IS_EMPTY(actual), false);
}

void test_first_item() {
    auto actual = MAKE_DARRAY(IntArray, 3, 4, 5);
    ASSERT_EQUAL_INT("test_first_item", FIRST_ITEM(actual), 3);
}

void test_last_item() {
    auto actual = MAKE_DARRAY(IntArray, 3, 4, 5);
    ASSERT_EQUAL_INT("test_last_item", LAST_ITEM(actual), 5);
}

void test_for_each() {
    auto actual = MAKE_DARRAY(IntArray, 1, 2, 3);
    FOR_EACH(it, actual) {
        *it = square(*it);
    }
    auto expected = MAKE_DARRAY(IntArray, 1, 4, 9);
    ASSERT_EQUAL_RANGE("test_for_each", actual, expected);  
}

void test_for_each2() {
    auto input = MAKE_DARRAY(IntArray, 1, 2, 3);
    auto actual = MAKE_DARRAY(IntArray, 0, 0, 0);
    auto expected = MAKE_DARRAY(IntArray, 1, 4, 9);
    FOR_EACH2(a, b, actual, input) {
        *a = square(*b);
    }
    ASSERT_EQUAL_RANGE("test_for_each2", actual, expected);
}

void test_for_each3() {
    auto left = MAKE_DARRAY(IntArray, 1, 2, 3);
    auto right = MAKE_DARRAY(IntArray, 0, 1, 2);
    auto actual = MAKE_DARRAY(IntArray, 0, 0, 0);
    auto expected = MAKE_DARRAY(IntArray, 1, 3, 5);
    FOR_EACH3(a, b, c, actual, left, right) {
        *a = add(*b, *c);
    }
    ASSERT_EQUAL_RANGE("test_for_each3", actual, expected);
}

void test_for_each_backward0() {
    auto inputs = (IntArray){};
    auto actual = (IntArray){};
    FOR_EACH_BACKWARD(it, inputs) {
        APPEND(actual, *it);
    }
    auto expected = (IntArray){};
    ASSERT_EQUAL_RANGE("test_for_each_backward0", actual, expected);
}

void test_for_each_backward1() {
    auto inputs = MAKE_DARRAY(IntArray, 1);
    auto actual = (IntArray){};
    FOR_EACH_BACKWARD(it, inputs) {
        APPEND(actual, *it);
    }
    auto expected = MAKE_DARRAY(IntArray, 1);
    ASSERT_EQUAL_RANGE("test_for_each_backward1", actual, expected);
}

void test_for_each_backward2() {
    auto inputs = MAKE_DARRAY(IntArray, 1, 2);
    auto actual = (IntArray){};
    FOR_EACH_BACKWARD(it, inputs) {
        APPEND(actual, *it);
    }
    auto expected = MAKE_DARRAY(IntArray, 2, 1);
    ASSERT_EQUAL_RANGE("test_for_each_backward2", actual, expected);
}

void test_for_each_backward3() {
    auto inputs = MAKE_DARRAY(IntArray, 1, 2, 3);
    auto actual = (IntArray){};
    FOR_EACH_BACKWARD(it, inputs) {
        APPEND(actual, *it);
    }
    auto expected = MAKE_DARRAY(IntArray, 3, 2, 1);
    ASSERT_EQUAL_RANGE("test_for_each_backward3", actual, expected);
}

void test_for_index() {
    auto actual = MAKE_DARRAY(IntArray, 1, 2, 3);
    FOR_INDEX(i, actual) {
        actual.data[i] = square(actual.data[i]);
    }
    auto expected = MAKE_DARRAY(IntArray, 1, 4, 9);
    ASSERT_EQUAL_RANGE("test_for_index", actual, expected);
}

void test_for_min() {
    auto actual = MAKE_DARRAY(IntArray, 1, -3, 2);
    FOR_MIN(it, actual) {
        ASSERT_EQUAL_INT("test_for_min element", *it, -3);
    }
    ASSERT_NOT_EQUAL_POINTER("test_for_min iterator", it, END_POINTER(actual));
}

void test_for_max() {
    auto actual = MAKE_DARRAY(IntArray, 1, 3, 2);
    FOR_MAX(it, actual) {
        ASSERT_EQUAL_INT("test_for_max element", *it, 3);
    }
    ASSERT_NOT_EQUAL_POINTER("test_for_max iterator", it, END_POINTER(actual));
}

void test_are_equal() {
    auto a = MAKE_DARRAY(IntArray, 1, 2, 3);
    auto b = MAKE_DARRAY(IntArray, 1, 2, 3);
    ASSERT_BOOL("test_are_equal", ARE_EQUAL(a, b));
}

void test_are_equal_not() {
    auto a = MAKE_DARRAY(IntArray, 1, 2, 3);
    auto b = MAKE_DARRAY(IntArray, 1, 2, 4);
    ASSERT_BOOL("test_are_equal", !ARE_EQUAL(a, b));
}

void test_are_equal_empty() {
    auto a = MAKE_DARRAY(IntArray);
    auto b = MAKE_DARRAY(IntArray);
    ASSERT_BOOL("test_are_equal", ARE_EQUAL(a, b));
}

void test_are_equal_different_count() {
    auto a = MAKE_DARRAY(IntArray, 1, 2);
    auto b = MAKE_DARRAY(IntArray, 1, 2, 3);
    ASSERT_BOOL("test_are_equal", !ARE_EQUAL(a, b));
}

void test_fill() {
    auto actual = MAKE_DARRAY(IntArray, 1, 2, 3);
    FILL(actual, 3);
    auto expected = MAKE_DARRAY(IntArray, 3, 3, 3);
    ASSERT_EQUAL_RANGE("test_fill", actual, expected);
}

void test_drop_front() {
    auto actual = MAKE_RANGE(IntRange, -1, 4, -2, 1);
    DROP_FRONT(actual);
    auto expected = MAKE_RANGE(IntRange, 4, -2, 1);
    ASSERT_EQUAL_RANGE("drop_front", actual, expected);
}

void test_drop_back() {
    auto actual = MAKE_RANGE(IntRange, -1, 4, -2, 1);
    DROP_BACK(actual);
    auto expected = MAKE_RANGE(IntRange, -1, 4, -2);
    ASSERT_EQUAL_RANGE("drop_back", actual, expected);
}

void test_drop_front_while() {
    auto range = MAKE_RANGE(IntRange, -1, 4, -2, 1);
    DROP_FRONT_WHILE(range, is_negative);
    ASSERT_EQUAL_INT("DROP_FRONT_WHILE", FIRST_ITEM(range), 4);
    DROP_FRONT_WHILE(range, is_positive);
    ASSERT_EQUAL_INT("DROP_FRONT_WHILE", FIRST_ITEM(range), -2);
    DROP_FRONT_WHILE(range, is_zero);
    ASSERT_EQUAL_INT("DROP_FRONT_WHILE", FIRST_ITEM(range), -2);
}

void test_drop_front_while_item() {
    auto range = MAKE_RANGE(IntRange, -1, 4, -2, 1);
    DROP_FRONT_WHILE_ITEM(range, -1);
    ASSERT_EQUAL_INT("DROP_FRONT_WHILE_ITEM", FIRST_ITEM(range), 4);
    DROP_FRONT_WHILE_ITEM(range, 4);
    ASSERT_EQUAL_INT("DROP_FRONT_WHILE_ITEM", FIRST_ITEM(range), -2);
    DROP_FRONT_WHILE_ITEM(range, 0);
    ASSERT_EQUAL_INT("DROP_FRONT_WHILE_ITEM", FIRST_ITEM(range), -2);
}

void test_drop_front_until() {
    auto range = MAKE_RANGE(IntRange, -1, 4, -2, 1);
    DROP_FRONT_UNTIL(range, is_positive);
    ASSERT_EQUAL_INT("DROP_FRONT_UNTIL", FIRST_ITEM(range), 4);
    DROP_FRONT_UNTIL(range, is_negative);
    ASSERT_EQUAL_INT("DROP_FRONT_UNTIL", FIRST_ITEM(range), -2);
    DROP_FRONT_UNTIL(range, is_zero);
    ASSERT_EQUAL_POINTER("DROP_FRONT_UNTIL", BEGIN_POINTER(range), END_POINTER(range));
}

void test_drop_front_until_item() {
    auto range = MAKE_RANGE(IntRange, -1, 4, -2, 1);
    DROP_FRONT_UNTIL_ITEM(range, 4);
    ASSERT_EQUAL_INT("DROP_FRONT_UNTIL_ITEM", FIRST_ITEM(range), 4);
    DROP_FRONT_UNTIL_ITEM(range, -2);
    ASSERT_EQUAL_INT("DROP_FRONT_UNTIL_ITEM", FIRST_ITEM(range), -2);
    DROP_FRONT_UNTIL_ITEM(range, 0);
    ASSERT_EQUAL_POINTER("DROP_FRONT_UNTIL_ITEM", BEGIN_POINTER(range), END_POINTER(range));
}

void test_drop_back_while() {
    auto range = MAKE_RANGE(IntRange, -1, 4, -2, 1);
    DROP_BACK_WHILE(range, is_positive);
    ASSERT_EQUAL_INT("DROP_BACK_WHILE", LAST_ITEM(range), -2);
    DROP_BACK_WHILE(range, is_negative);
    ASSERT_EQUAL_INT("DROP_BACK_WHILE", LAST_ITEM(range), 4);
    DROP_BACK_WHILE(range, is_zero);
    ASSERT_EQUAL_INT("DROP_BACK_WHILE", LAST_ITEM(range), 4);
}

void test_drop_back_while_item() {
    auto range = MAKE_RANGE(IntRange, -1, 4, -2, 1);
    DROP_BACK_WHILE_ITEM(range, 1);
    ASSERT_EQUAL_INT("DROP_BACK_WHILE_ITEM", LAST_ITEM(range), -2);
    DROP_BACK_WHILE_ITEM(range, -2);
    ASSERT_EQUAL_INT("DROP_BACK_WHILE_ITEM", LAST_ITEM(range), 4);
    DROP_BACK_WHILE_ITEM(range, 0);
    ASSERT_EQUAL_INT("DROP_BACK_WHILE_ITEM", LAST_ITEM(range), 4);
}

void test_drop_back_until() {
    auto range = MAKE_RANGE(IntRange, -1, 4, -2, 1);
    DROP_BACK_UNTIL(range, is_negative);
    ASSERT_EQUAL_INT("DROP_BACK_UNTIL", LAST_ITEM(range), -2);
    DROP_BACK_UNTIL(range, is_positive);
    ASSERT_EQUAL_INT("DROP_BACK_UNTIL", LAST_ITEM(range), 4);
    DROP_BACK_UNTIL(range, is_zero);
    ASSERT_EQUAL_SIZE("DROP_BACK_UNTIL", range.count, 0);
}

void test_drop_back_until_item() {
    auto range = MAKE_RANGE(IntRange, -1, 4, -2, 1);
    DROP_BACK_UNTIL_ITEM(range, -2);
    ASSERT_EQUAL_INT("DROP_BACK_UNTIL_ITEM", LAST_ITEM(range), -2);
    DROP_BACK_UNTIL_ITEM(range, 4);
    ASSERT_EQUAL_INT("DROP_BACK_UNTIL_ITEM", LAST_ITEM(range), 4);
    DROP_BACK_UNTIL_ITEM(range, 0);
    ASSERT_EQUAL_SIZE("DROP_BACK_UNTIL_ITEM", range.count, 0);
}

void test_erase_index1() {
    auto actual = MAKE_DARRAY(IntArray, 9);
    ERASE_INDEX(actual, 0);
    auto expected = (IntArray){};
    ASSERT_EQUAL_RANGE("ERASE_INDEX 1", actual, expected);
}

void test_erase_index_ordered1() {
    auto actual = MAKE_DARRAY(IntArray, 9);
    ERASE_INDEX_ORDERED(actual, 0);
    auto expected = (IntArray){};
    ASSERT_EQUAL_RANGE("ERASE_INDEX 1", actual, expected);
}

void test_erase_index2a() {
    auto actual = MAKE_DARRAY(IntArray, 9, 8);
    ERASE_INDEX(actual, 0);
    auto expected = MAKE_DARRAY(IntArray, 8);
    ASSERT_EQUAL_RANGE("ERASE_INDEX 2a", actual, expected);
}

void test_erase_index_ordered2a() {
    auto actual = MAKE_DARRAY(IntArray, 9, 8);
    ERASE_INDEX_ORDERED(actual, 0);
    auto expected = MAKE_DARRAY(IntArray, 8);
    ASSERT_EQUAL_RANGE("ERASE_INDEX 2a", actual, expected);
}

void test_erase_index2b() {
    auto actual = MAKE_DARRAY(IntArray, 9, 8);
    ERASE_INDEX(actual, 1);
    auto expected = MAKE_DARRAY(IntArray, 9);
    ASSERT_EQUAL_RANGE("ERASE_INDEX 2b", actual, expected);
}

void test_erase_index_ordered2b() {
    auto actual = MAKE_DARRAY(IntArray, 9, 8);
    ERASE_INDEX_ORDERED(actual, 1);
    auto expected = MAKE_DARRAY(IntArray, 9);
    ASSERT_EQUAL_RANGE("ERASE_INDEX 2b", actual, expected);
}

void test_erase_index3a() {
    auto actual = MAKE_DARRAY(IntArray, 9, 8, 7);
    ERASE_INDEX(actual, 0);
    auto expected = MAKE_DARRAY(IntArray, 7, 8);
    ASSERT_EQUAL_RANGE("ERASE_INDEX 3a", actual, expected);
}

void test_erase_index_ordered3a() {
    auto actual = MAKE_DARRAY(IntArray, 9, 8, 7);
    ERASE_INDEX_ORDERED(actual, 0);
    auto expected = MAKE_DARRAY(IntArray, 8, 7);
    ASSERT_EQUAL_RANGE("ERASE_INDEX 3a", actual, expected);
}

void test_erase_index3b() {
    auto actual = MAKE_DARRAY(IntArray, 9, 8, 7);
    ERASE_INDEX(actual, 1);
    auto expected = MAKE_DARRAY(IntArray, 9, 7);
    ASSERT_EQUAL_RANGE("ERASE_INDEX 3b", actual, expected);
}

void test_erase_index_ordered3b() {
    auto actual = MAKE_DARRAY(IntArray, 9, 8, 7);
    ERASE_INDEX_ORDERED(actual, 1);
    auto expected = MAKE_DARRAY(IntArray, 9, 7);
    ASSERT_EQUAL_RANGE("ERASE_INDEX 3b", actual, expected);
}


void test_erase_index3c() {
    auto actual = MAKE_DARRAY(IntArray, 9, 8, 7);
    ERASE_INDEX(actual, 2);
    auto expected = MAKE_DARRAY(IntArray, 9, 8);
    ASSERT_EQUAL_RANGE("ERASE_INDEX 3b", actual, expected);
}

void test_erase_index_ordered3c() {
    auto actual = MAKE_DARRAY(IntArray, 9, 8, 7);
    ERASE_INDEX_ORDERED(actual, 2);
    auto expected = MAKE_DARRAY(IntArray, 9, 8);
    ASSERT_EQUAL_RANGE("ERASE_INDEX 3b", actual, expected);
}

void test_erase_if_unallocated() {
    IntArray array;
    array.data = 0;
    array.count = 0;
    array.capacity = 0;

    ERASE_IF(array, is_zero);
    ASSERT_EQUAL_SIZE("ERASE_IF EMPTY", array.count, 0);
    ASSERT_EQUAL_RANGE("ERASE_IF EMPTY", array, array);
    FREE_DARRAY(array);
}

void test_erase_if_empty() {
    auto array = (IntArray){};
    ERASE_IF(array, is_zero);
    ASSERT_EQUAL_SIZE("ERASE_IF EMPTY", array.count, 0);
    ASSERT_EQUAL_RANGE("ERASE_IF EMPTY", array, array);
    FREE_DARRAY(array);
}

void test_erase_if0() {
    auto actual = MAKE_DARRAY(IntArray, 0);
    ERASE_IF(actual, is_zero);
    auto expected = (IntArray){};
    ASSERT_EQUAL_RANGE("ERASE_IF 0", actual, expected);
}

void test_erase_if1() {
    auto actual = MAKE_DARRAY(IntArray, 1);
    ERASE_IF(actual, is_zero);
    auto expected = MAKE_DARRAY(IntArray, 1);
    ASSERT_EQUAL_RANGE("ERASE_IF 1", actual, expected);
}

void test_erase_if11() {
    auto actual = MAKE_DARRAY(IntArray, 1, 1);
    ERASE_IF(actual, is_zero);
    auto expected = MAKE_DARRAY(IntArray, 1, 1);
    ASSERT_EQUAL_RANGE("ERASE_IF 11", actual, expected);
}

void test_erase_if00() {
    auto actual = MAKE_DARRAY(IntArray, 0, 0);
    ERASE_IF(actual, is_zero);
    auto expected = (IntArray){};
    ASSERT_EQUAL_RANGE("ERASE_IF 00", actual, expected);
}

void test_erase_if10() {
    auto actual = MAKE_DARRAY(IntArray, 1, 0);
    ERASE_IF(actual, is_zero);
    auto expected = MAKE_DARRAY(IntArray, 1);
    ASSERT_EQUAL_RANGE("ERASE_IF 10", actual, expected);
}

void test_erase_if01() {
    auto actual = MAKE_DARRAY(IntArray, 0, 1);
    ERASE_IF(actual, is_zero);
    auto expected = MAKE_DARRAY(IntArray, 1);
    ASSERT_EQUAL_RANGE("ERASE_IF 01", actual, expected);
}

void test_erase_if101() {
    auto actual = MAKE_DARRAY(IntArray, 1, 0, 1);
    ERASE_IF(actual, is_zero);
    auto expected = MAKE_DARRAY(IntArray, 1, 1);
    ASSERT_EQUAL_RANGE("ERASE_IF 101", actual, expected);
}

void test_erase_if010() {
    auto actual = MAKE_DARRAY(IntArray, 0, 1, 0);
    ERASE_IF(actual, is_zero);
    auto expected = MAKE_DARRAY(IntArray, 1);
    ASSERT_EQUAL_RANGE("ERASE_IF 010", actual, expected);
}

void test_erase_if() {
    auto actual = MAKE_DARRAY(IntArray, -1, 4, -2, 1);
    ERASE_IF(actual, is_positive);
    auto expected = MAKE_DARRAY(IntArray, -1, -2);
    ASSERT_EQUAL_RANGE("ERASE_IF", actual, expected);
}

void test_append() {
    IntArray array;
    INIT_DARRAY(array, 0, 0);
    ASSERT_EQUAL_SIZE("APPEND", array.count, 0);
    ASSERT_EQUAL_SIZE("APPEND", array.capacity, 0);
    APPEND(array, 1);
    ASSERT_EQUAL_SIZE("APPEND", array.count, 1);
    ASSERT_EQUAL_SIZE("APPEND", array.capacity, 1);
    APPEND(array, 2);
    ASSERT_EQUAL_SIZE("APPEND", array.count, 2);
    ASSERT_EQUAL_SIZE("APPEND", array.capacity, 2);
    APPEND(array, 3);
    ASSERT_EQUAL_SIZE("APPEND", array.count, 3);
    ASSERT_EQUAL_SIZE("APPEND", array.capacity, 4);
    APPEND(array, 4);
    ASSERT_EQUAL_SIZE("APPEND", array.count, 4);
    ASSERT_EQUAL_SIZE("APPEND", array.capacity, 4);
    APPEND(array, 5);
    ASSERT_EQUAL_SIZE("APPEND", array.count, 5);
    ASSERT_EQUAL_SIZE("APPEND", array.capacity, 8);

    auto expected = MAKE_DARRAY(IntArray, 1, 2, 3, 4, 5);
    ASSERT_EQUAL_RANGE("APPEND", array, expected);
    FREE_DARRAY(array);
}

void test_prepend() {
    IntArray array;
    INIT_DARRAY(array, 0, 0);
    ASSERT_EQUAL_SIZE("PREPEND", array.count, 0);
    ASSERT_EQUAL_SIZE("PREPEND", array.capacity, 0);
    PREPEND(array, 1);
    ASSERT_EQUAL_SIZE("PREPEND", array.count, 1);
    ASSERT_EQUAL_SIZE("PREPEND", array.capacity, 1);
    PREPEND(array, 2);
    ASSERT_EQUAL_SIZE("PREPEND", array.count, 2);
    ASSERT_EQUAL_SIZE("PREPEND", array.capacity, 2);
    PREPEND(array, 3);
    ASSERT_EQUAL_SIZE("PREPEND", array.count, 3);
    ASSERT_EQUAL_SIZE("PREPEND", array.capacity, 4);
    PREPEND(array, 4);
    ASSERT_EQUAL_SIZE("PREPEND", array.count, 4);
    ASSERT_EQUAL_SIZE("PREPEND", array.capacity, 4);
    PREPEND(array, 5);
    ASSERT_EQUAL_SIZE("PREPEND", array.count, 5);
    ASSERT_EQUAL_SIZE("PREPEND", array.capacity, 8);

    auto expected = MAKE_DARRAY(IntArray, 5, 4, 3, 2, 1);
    ASSERT_EQUAL_RANGE("APPEND", array, expected);
    FREE_DARRAY(array);
}

void test_insert_index0() {
    auto actual = (IntArray){};
    INSERT_INDEX(actual, 0, 1);
    auto expected = MAKE_DARRAY(IntArray, 1);
    ASSERT_EQUAL_RANGE("INSERT_INDEX0", actual, expected);
}

void test_insert_index1a() {
    auto actual = MAKE_DARRAY(IntArray, 1);
    INSERT_INDEX(actual, 0, 2);
    auto expected = MAKE_DARRAY(IntArray, 2, 1);
    ASSERT_EQUAL_RANGE("INSERT_INDEX1a", actual, expected);
}

void test_insert_index1b() {
    auto actual = MAKE_DARRAY(IntArray, 1);
    INSERT_INDEX(actual, 1, 2);
    auto expected = MAKE_DARRAY(IntArray, 1, 2);
    ASSERT_EQUAL_RANGE("INSERT_INDEX1b", actual, expected);
}

void test_insert_index2a() {
    auto actual = MAKE_DARRAY(IntArray, 1, 2);
    INSERT_INDEX(actual, 0, 3);
    auto expected = MAKE_DARRAY(IntArray, 3, 1, 2);
    ASSERT_EQUAL_RANGE("INSERT_INDEX2a", actual, expected);
}

void test_insert_index2b() {
    auto actual = MAKE_DARRAY(IntArray, 1, 2);
    INSERT_INDEX(actual, 1, 3);
    auto expected = MAKE_DARRAY(IntArray, 1, 3, 2);
    ASSERT_EQUAL_RANGE("INSERT_INDEX2b", actual, expected);
}

void test_insert_index2c() {
    auto actual = MAKE_DARRAY(IntArray, 1, 2);
    INSERT_INDEX(actual, 2, 3);
    auto expected = MAKE_DARRAY(IntArray, 1, 2, 3);
    ASSERT_EQUAL_RANGE("INSERT_INDEX2b", actual, expected);
}

void test_concat() {
    IntArray target;
    INIT_DARRAY(target, 0, 0);
    
    auto source = MAKE_DARRAY(IntArray, 1, 2, 3);
    
    ASSERT_EQUAL_SIZE("CONCAT", target.count, 0);
    ASSERT_EQUAL_SIZE("CONCAT", target.capacity, 0);

    CONCAT(target, source);
    auto expected0 = MAKE_DARRAY(IntArray, 1, 2, 3);
    ASSERT_EQUAL_RANGE("CONCAT", target, expected0);
    ASSERT_EQUAL_SIZE("CONCAT", target.capacity, 4);

    CONCAT(target, source);
    auto expected1 = MAKE_DARRAY(IntArray, 1, 2, 3, 1, 2, 3);
    ASSERT_EQUAL_RANGE("CONCAT", target, expected1);
    ASSERT_EQUAL_SIZE("CONCAT", target.capacity, 8);
}

void test_for_x_y() {
    Image actual;
    INIT_2D_ARRAY(actual, 2, 3);
    auto i = 0;
    FOR_Y(y, actual) {
        FOR_X(x, actual) {
            AT_XY(actual, x, y) = i;
            ++i;
        }
    }
    auto expected = MAKE_DARRAY(IntArray,
        0, 1,
        2, 3,
        4, 5,
    );
    ASSERT_EQUAL_RANGE("test_for_x_y", actual, expected);
}

void test_for_x_y_z() {
    Voxels actual;
    INIT_3D_ARRAY(actual, 2, 3, 4);
    auto i = 0;
    FOR_Z(z, actual) {
        FOR_Y(y, actual) {
            FOR_X(x, actual) {
                AT_XYZ(actual, x, y, z) = i;
                ++i;
            }
        }
    }
    auto expected = MAKE_DARRAY(IntArray,
        0, 1, 2, 3,
        4, 5, 6, 7,
        8, 9, 10, 11,
        
        12, 13, 14, 15,
        16, 17, 18, 19,
        20, 21, 22, 23,
    );
    ASSERT_EQUAL_RANGE("test_for_x_y_z", actual, expected);
}

void test_flip_image_x() {
    Image actual;
    INIT_2D_ARRAY(actual, 2, 3);
    auto i = 0;
    FOR_Y(y, actual) {
        FOR_X(x, actual) {
            AT_XY(actual, x, y) = i;
            ++i;
        }
    }
    FLIP_IMAGE_X(actual);
    auto expected = MAKE_DARRAY(IntArray,
        1, 0,
        3, 2,
        5, 4,
    );
    ASSERT_EQUAL_RANGE("test_flip_image_x", actual, expected);
}

void test_flip_image_y() {
    Image actual;
    INIT_2D_ARRAY(actual, 2, 3);
    auto i = 0;
    FOR_Y(y, actual) {
        FOR_X(x, actual) {
            AT_XY(actual, x, y) = i;
            ++i;
        }
    }
    FLIP_IMAGE_Y(actual);
    auto expected = MAKE_DARRAY(IntArray,
        4, 5,
        2, 3,
        0, 1,
    );
    ASSERT_EQUAL_RANGE("test_flip_image_y", actual, expected);
}

void test_string_view() {
    ASSERT_EQUAL_INT("test_string_view", STRING_VIEW("").count, 0);
    ASSERT_EQUAL_INT("test_string_view", STRING_VIEW("a").count, 1);
    ASSERT_EQUAL_INT("test_string_view", STRING_VIEW("ab").count, 2);
    ASSERT_EQUAL_INT("test_string_view", STRING_VIEW("\n").count, 1);
    ASSERT_EQUAL_INT("test_string_view", STRING_VIEW("\0").count, 0);
}

void test_concat_cstring() {
    auto s = (StringBuilder){};

    s = (StringBuilder){};
    CONCAT_CSTRING(s, "");
    ASSERT_STRING_BUILDER("test_concat_cstring 0", s, "", 0, 1);

    s = (StringBuilder){};
    CONCAT_CSTRING(s, "");
    CONCAT_CSTRING(s, "");
    ASSERT_STRING_BUILDER("test_concat_cstring 1", s, "", 0, 1);

    s = (StringBuilder){};
    CONCAT_CSTRING(s, "a");
    CONCAT_CSTRING(s, "");
    ASSERT_STRING_BUILDER("test_concat_cstring 2", s, "a", 1, 2);

    s = (StringBuilder){};
    CONCAT_CSTRING(s, "");
    CONCAT_CSTRING(s, "a");
    ASSERT_STRING_BUILDER("test_concat_cstring 3", s, "a", 1, 2);
}

void test_concat_string() {
    auto s = (StringBuilder){};
    
    s = (StringBuilder){};
    CONCAT_STRING(s, "");
    ASSERT_STRING_BUILDER("test_concat_string 0", s, "", 0, 1);

    s = (StringBuilder){};
    CONCAT_STRING(s, "a");
    ASSERT_STRING_BUILDER("test_concat_string 1", s, "a", 1, 2);

    s = (StringBuilder){};
    CONCAT_STRING(s, "ab");
    ASSERT_STRING_BUILDER("test_concat_string 2", s, "ab", 2, 4);

    s = (StringBuilder){};
    CONCAT_STRING(s, "%i%s\n%i", 1, "ab", 99);
    ASSERT_STRING_BUILDER("test_concat_string 3", s, "1ab\n99", 6, 8);

    s = (StringBuilder){};
    CONCAT_STRING(s, "");
    CONCAT_STRING(s, "");
    ASSERT_STRING_BUILDER("test_concat_string 4", s, "", 0, 1);
    
    s = (StringBuilder){};
    CONCAT_STRING(s, "");
    CONCAT_STRING(s, "a");
    ASSERT_STRING_BUILDER("test_concat_string 5", s, "a", 1, 2);

    s = (StringBuilder){};
    CONCAT_STRING(s, "a");
    CONCAT_STRING(s, "");
    ASSERT_STRING_BUILDER("test_concat_string 6", s, "a", 1, 2);

    s = (StringBuilder){};
    CONCAT_STRING(s, "a");
    CONCAT_STRING(s, "b");
    ASSERT_STRING_BUILDER("test_concat_string 7", s, "ab", 2, 4);
    
    s = (StringBuilder){};
    CONCAT_STRING(s, "a");
    CONCAT_STRING(s, "bb");
    CONCAT_STRING(s, "ccc");
    ASSERT_STRING_BUILDER("test_concat_string 8", s, "abbccc", 6, 8);
}

void test_table_set_key_value_duplicates() {
    auto table = (TableIntInt){};
    SET_KEY_VALUE(1, 2, table);
    SET_KEY_VALUE(1, 3, table);
    auto product = 1;
    FOR_EACH_TABLE(item, table) {
        product *= item->value;
    }
    ASSERT_EQUAL_INT("test_table_set_key_value_duplicates", product, 3);
}

void test_table_set_keys_value_duplicates() {
    auto table = (TableIntArrayInt){};
    auto keys = (IntArray){};
    APPEND(keys, 1);
    APPEND(keys, 2);
    SET_RANGE_KEY_VALUE(keys, 2, table);
    SET_RANGE_KEY_VALUE(keys, 3, table);
    auto product = 1;
    FOR_EACH_TABLE(item, table) {
        product *= item->value;
    }
    ASSERT_EQUAL_INT("test_table_set_keys_value_duplicates", product, 3);
}

void test_table_set_key_value() {
    auto table = (TableIntInt){};
    SET_KEY_VALUE(1, 2, table);
    SET_KEY_VALUE(2, 3, table);
    SET_KEY_VALUE(3, 0, table);
    SET_KEY_VALUE(3, 5, table);
    auto product = 1;
    FOR_EACH_TABLE(item, table) {
        product *= item->value;
    }
    ASSERT_EQUAL_INT("test_table_set_key_value", product, 30);
}

void test_table_set_keys_value() {
    auto table = (TableIntArrayInt){};
    auto keys = (IntArray){};
    APPEND(keys, 1);
    SET_RANGE_KEY_VALUE(keys, 2, table);
    APPEND(keys, 2);
    SET_RANGE_KEY_VALUE(keys, 3, table);
    APPEND(keys, 3);
    SET_RANGE_KEY_VALUE(keys, 0, table);
    SET_RANGE_KEY_VALUE(keys, 5, table);

    auto product = 1;
    FOR_EACH_TABLE(item, table) {
        product *= item->value;
    }
    ASSERT_EQUAL_INT("test_table_set_keys_value", product, 30);
}

void test_table_missing_key() {
    auto table = (TableIntInt){};
    auto value = 0;
    GET_KEY_VALUE(2, value, table);
    ASSERT_EQUAL_INT("test_table_missing_key", value, 0);
}

void test_table_missing_keys() {
    auto table = (TableIntArrayInt){};
    auto keys = (IntArray){};
    APPEND(keys, 1);
    auto value = 0;
    GET_RANGE_KEY_VALUE(keys, value, table);
    ASSERT_EQUAL_INT("test_table_missing_keys", value, 0);
}

void test_table_available_key() {
    auto table = (TableIntInt){};
    SET_KEY_VALUE(2, 5, table);
    auto value = 0;
    GET_KEY_VALUE(2, value, table);
    ASSERT_EQUAL_INT("test_table_available_key", value, 5);
}

void test_table_available_keys() {
    auto table = (TableIntArrayInt){};
    auto keys = (IntArray){};
    APPEND(keys, 1);
    SET_RANGE_KEY_VALUE(keys, 5, table);
    auto value = 0;
    GET_RANGE_KEY_VALUE(keys, value, table);
    ASSERT_EQUAL_INT("test_table_available_2_keys", value, 5);
}

int main() {
    test_2d_array();
    test_3d_array();
    
    test_first_item();
    test_last_item();
    test_is_empty();
    
    test_for_each();
    test_for_each2();
    test_for_each3();
    test_for_index();

    test_for_each_backward0();
    test_for_each_backward1();
    test_for_each_backward2();
    test_for_each_backward3();

    test_for_min();
    test_for_max();

    test_are_equal();
    test_are_equal_not();
    test_are_equal_empty();
    test_are_equal_different_count();

    test_fill();
    
    test_drop_front();
    test_drop_back();
    
    test_drop_front_while();
    test_drop_front_until();
    test_drop_back_while();
    test_drop_back_until();

    test_drop_front_while_item();
    test_drop_front_until_item();
    test_drop_back_while_item();
    test_drop_back_until_item();

    test_erase_index1();
    test_erase_index2a();
    test_erase_index2b();
    test_erase_index3a();
    test_erase_index3b();
    test_erase_index3c();

    test_erase_index_ordered1();
    test_erase_index_ordered2a();
    test_erase_index_ordered2b();
    test_erase_index_ordered3a();
    test_erase_index_ordered3b();
    test_erase_index_ordered3c();

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
    test_prepend();
    test_concat();

    test_insert_index0();
    test_insert_index1a();
    test_insert_index1b();
    test_insert_index2a();
    test_insert_index2b();
    test_insert_index2c();

    test_for_x_y();
    test_for_x_y_z();

    test_flip_image_x();
    test_flip_image_y();

    test_string_view();
    test_concat_cstring();
    test_concat_string();

    test_table_set_key_value_duplicates();
    test_table_set_keys_value_duplicates();
    
    test_table_set_key_value();
    test_table_set_keys_value();
    
    test_table_missing_key();
    test_table_missing_keys();
    
    test_table_available_key();
    test_table_available_keys();
    
    summarize_tests();
    
    return 0;
}
