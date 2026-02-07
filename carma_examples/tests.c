#include <stdbool.h>
#include <stdio.h>

#include <carma/carma.h>
#include <carma/carma_assert.h>
#include <carma/carma_parse.h>
#include <carma/carma_json_serialize.h>
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

#define ASSERT_EQUAL_CARMA_STRINGS(description, left_string, right_string) do { \
    global_assert_count++; \
    if (ARE_EQUAL(left_string, right_string)) { \
        printf("%s ok\n", (description)); \
    } else { \
        printf("%s ", (description)); \
        PRINT_CARMA_STRING(left_string); \
        printf(" != "); \
        PRINT_CARMA_STRING(right_string); \
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

void test_for_each_backward2() {
    auto input0 = MAKE_DARRAY(IntArray, 1, 2, 4);
    auto input1 = MAKE_DARRAY(IntArray, 8, 16, 32);
    auto expected = MAKE_DARRAY(IntArray, 28, 14, 7);
    auto actual = MAKE_DARRAY(IntArray);
    FOR_EACH_BACKWARD2(a, b, input0, input1) {
        auto item = *b - *a;
        APPEND(actual, item);
    }
    ASSERT_EQUAL_RANGE("test_for_each_backward2", actual, expected);
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

void test_for_each_backward_a() {
    auto inputs = (IntArray){};
    auto actual = (IntArray){};
    FOR_EACH_BACKWARD(it, inputs) {
        APPEND(actual, *it);
    }
    auto expected = (IntArray){};
    ASSERT_EQUAL_RANGE("test_for_each_backward0", actual, expected);
}

void test_for_each_backward_b() {
    auto inputs = MAKE_DARRAY(IntArray, 1);
    auto actual = (IntArray){};
    FOR_EACH_BACKWARD(it, inputs) {
        APPEND(actual, *it);
    }
    auto expected = MAKE_DARRAY(IntArray, 1);
    ASSERT_EQUAL_RANGE("test_for_each_backward1", actual, expected);
}

void test_for_each_backward_c() {
    auto inputs = MAKE_DARRAY(IntArray, 1, 2);
    auto actual = (IntArray){};
    FOR_EACH_BACKWARD(it, inputs) {
        APPEND(actual, *it);
    }
    auto expected = MAKE_DARRAY(IntArray, 2, 1);
    ASSERT_EQUAL_RANGE("test_for_each_backward2", actual, expected);
}

void test_for_each_backward_d() {
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

void test_sub_range0() {
    auto source = MAKE_DARRAY(IntArray);
    auto actual = SUB_RANGE(source, 0, 0);
    auto expected = MAKE_DARRAY(IntArray);
    ASSERT_EQUAL_RANGE("test_sub_range0", actual, expected);
}

void test_sub_range1a() {
    auto source = MAKE_DARRAY(IntArray, 1);
    auto actual = SUB_RANGE(source, 0, 0);
    auto expected = MAKE_DARRAY(IntArray);
    ASSERT_EQUAL_RANGE("test_sub_range1a", actual, expected);
}

void test_sub_range1b() {
    auto source = MAKE_DARRAY(IntArray, 1);
    auto actual = SUB_RANGE(source, 1, 0);
    auto expected = MAKE_DARRAY(IntArray);
    ASSERT_EQUAL_RANGE("test_sub_range1b", actual, expected);
}

void test_sub_range1c() {
    auto source = MAKE_DARRAY(IntArray, 1);
    auto actual = SUB_RANGE(source, 0, 1);
    auto expected = MAKE_DARRAY(IntArray, 1);
    ASSERT_EQUAL_RANGE("test_sub_range1c", actual, expected);
}

void test_sub_range2a() {
    auto source = MAKE_DARRAY(IntArray, 1, 2);
    auto actual = SUB_RANGE(source, 0, 1);
    auto expected = MAKE_DARRAY(IntArray, 1);
    ASSERT_EQUAL_RANGE("test_sub_range2a", actual, expected);
}

void test_sub_range2b() {
    auto source = MAKE_DARRAY(IntArray, 1, 2);
    auto actual = SUB_RANGE(source, 1, 1);
    auto expected = MAKE_DARRAY(IntArray, 2);
    ASSERT_EQUAL_RANGE("test_sub_range2b", actual, expected);
}

void test_sub_range2c() {
    auto source = MAKE_DARRAY(IntArray, 1, 2);
    auto actual = SUB_RANGE(source, 0, 2);
    auto expected = MAKE_DARRAY(IntArray, 1, 2);
    ASSERT_EQUAL_RANGE("test_sub_range2c", actual, expected);
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

void test_copy0() {
    auto source = MAKE_DARRAY(IntArray);
    auto actual = MAKE_DARRAY(IntArray);
    COPY(source, actual);
    auto expected = MAKE_DARRAY(IntArray);
    ASSERT_EQUAL_RANGE("test_copy0", actual, expected);
}

void test_copy1() {
    auto source = MAKE_DARRAY(IntArray, 1);
    auto actual = MAKE_DARRAY(IntArray, 2);
    COPY(source, actual);
    auto expected = MAKE_DARRAY(IntArray, 1);
    ASSERT_EQUAL_RANGE("test_copy1", actual, expected);
}

void test_copy2() {
    auto source = MAKE_DARRAY(IntArray, 1, 2);
    auto actual = MAKE_DARRAY(IntArray, 3, 4);
    COPY(source, actual);
    auto expected = MAKE_DARRAY(IntArray, 1, 2);
    ASSERT_EQUAL_RANGE("test_copy2", actual, expected);
}

void test_copy_inplace() {
    auto actual = MAKE_DARRAY(IntArray, 1, 2, 3);
    auto source = SUB_RANGE(actual, 1, 2);
    auto target = SUB_RANGE(actual, 0, 2);
    COPY(source, target);
    auto expected = MAKE_DARRAY(IntArray, 2, 3, 3);
    ASSERT_EQUAL_RANGE("test_copy_inplace", actual, expected);
}

void test_copy_backward_inplace() {
    auto actual = MAKE_DARRAY(IntArray, 1, 2, 3);
    auto source = SUB_RANGE(actual, 0, 2);
    auto target = SUB_RANGE(actual, 1, 2);
    COPY_BACKWARD(source, target);
    auto expected = MAKE_DARRAY(IntArray, 1, 1, 2);
    ASSERT_EQUAL_RANGE("test_copy_backward_inplace", actual, expected);
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

void test_erase_back() {
    auto actual = MAKE_RANGE(IntRange, -1, 4, -2, 1);
    ERASE_BACK(actual);
    auto expected = MAKE_RANGE(IntRange, -1, 4, -2);
    ASSERT_EQUAL_RANGE("erase_back", actual, expected);
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

void test_erase_front() {
    auto actual = MAKE_DARRAY(IntArray, 1, 2, 3);
    ERASE_FRONT(actual);
    auto expected = MAKE_DARRAY(IntArray, 2, 3);
    ASSERT_EQUAL_RANGE("ERASE_FRONT", actual, expected);
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

void test_erase_many_ordered1() {
    auto actual = MAKE_DARRAY(IntArray, 9);
    ERASE_MANY_ORDERED(actual, 0, 1);
    auto expected = MAKE_DARRAY(IntArray);
    ASSERT_EQUAL_RANGE("ERASE_MANY_ORDERED 1", actual, expected);
}

void test_erase_many_ordered2() {
    auto actual = MAKE_DARRAY(IntArray, 9, 8, 7, 6, 5);
    ERASE_MANY_ORDERED(actual, 0, 2);
    auto expected = MAKE_DARRAY(IntArray, 7, 6, 5);
    ASSERT_EQUAL_RANGE("ERASE_MANY_ORDERED 1", actual, expected);
}

void test_erase_many_ordered3() {
    auto actual = MAKE_DARRAY(IntArray, 9, 8, 7, 6, 5);
    ERASE_MANY_ORDERED(actual, 3, 2);
    auto expected = MAKE_DARRAY(IntArray, 9, 8, 7);
    ASSERT_EQUAL_RANGE("ERASE_MANY_ORDERED 1", actual, expected);
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

void test_insert_range0a() {
    auto actual = MAKE_DARRAY(IntArray);
    auto part = MAKE_DARRAY(IntArray, 9);
    INSERT_RANGE(actual, 0, part);
    auto expected = MAKE_DARRAY(IntArray, 9);
    ASSERT_EQUAL_RANGE("INSERT_RANGE0a", actual, expected);
}

void test_insert_range0b() {
    auto actual = MAKE_DARRAY(IntArray);
    auto part = MAKE_DARRAY(IntArray, 8, 9);
    INSERT_RANGE(actual, 0, part);
    auto expected = MAKE_DARRAY(IntArray, 8, 9);
    ASSERT_EQUAL_RANGE("INSERT_RANGE0b", actual, expected);
}

void test_insert_range1a() {
    auto actual = MAKE_DARRAY(IntArray, 1);
    auto part = MAKE_DARRAY(IntArray, 8, 9);
    INSERT_RANGE(actual, 0, part);
    auto expected = MAKE_DARRAY(IntArray, 8, 9, 1);
    ASSERT_EQUAL_RANGE("INSERT_RANGE1a", actual, expected);
}

void test_insert_range1b() {
    auto actual = MAKE_DARRAY(IntArray, 1);
    auto part = MAKE_DARRAY(IntArray, 8, 9);
    INSERT_RANGE(actual, 1, part);
    auto expected = MAKE_DARRAY(IntArray, 1, 8, 9);
    ASSERT_EQUAL_RANGE("INSERT_RANGE1b", actual, expected);
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

void test_optional() {
    auto optional = (OptionalInt){};
    ASSERT_EQUAL_SIZE("Optional init", optional.count, 0);
    SET_OPTIONAL(optional, 3);
    ASSERT_EQUAL_SIZE("SET_OPTIONAL count", optional.count, 1);
    FOR_EACH(x, optional) {
        ASSERT_EQUAL_INT("Optional FOR_EACH 3", *x, 3);
    }
    SET_OPTIONAL(optional, 4);
    ASSERT_EQUAL_SIZE("Redo SET_OPTIONAL count", optional.count, 1);
    FOR_EACH(x, optional) {
        ASSERT_EQUAL_INT("Optional FOR_EACH 4", *x, 4);
    }
    (void)GET_OPTIONAL_OR_ABORT(optional);
    (void)GET_OPTIONAL_OR_EXIT(optional, "error message");
    CLEAR(optional);
    ASSERT_EQUAL_SIZE("Optional CLEAR", optional.count, 0);
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

void test_parse_int() {
    auto string = STRING_VIEW("15");
    auto value = PARSE_INT(string);
    ASSERT_EQUAL_INT("PARSE_INT", GET_OPTIONAL(value), 15);
    ASSERT_EQUAL_RANGE("PARSE_INT", string, (STRING_VIEW("")));
}

void test_parse_int_or_exit() {
    auto string = STRING_VIEW("15");
    auto value = PARSE_INT_OR_EXIT(string);
    ASSERT_EQUAL_INT("PARSE_INT_OR_EXIT", value, 15);
    ASSERT_EQUAL_RANGE("PARSE_INT_OR_EXIT", string, (STRING_VIEW("")));
}

void test_parse_int_as_string() {
    auto string = STRING_VIEW("+15 , 17");
    auto value = parse_int_as_string(&string);
    ASSERT_EQUAL_RANGE("parse_int_as_string item", value, (STRING_VIEW("+15")));
    ASSERT_EQUAL_RANGE("parse_int_as_string rest", string, (STRING_VIEW(" , 17")));
}

void test_parse_line() {
    auto string = STRING_VIEW("ab\ncd");
    auto value = PARSE_LINE(string);
    ASSERT_EQUAL_RANGE("PARSE_LINE", value, (STRING_VIEW("ab")));
    ASSERT_EQUAL_RANGE("PARSE_LINE", string, (STRING_VIEW("cd")));
}

void test_parse_whitespace() {
    auto string = STRING_VIEW(" \n a \n b");
    PARSE_WHITESPACE(string);
    ASSERT_EQUAL_RANGE("PARSE_WHITESPACE", string, (STRING_VIEW("a \n b")));
}

void test_parse_quoted_string() {
    auto string = STRING_VIEW("\"Hello \\\"name\\\" \\n and welcome!\", 1, 2");
    auto value = PARSE_QUOTED_STRING(string);
    ASSERT_EQUAL_RANGE("PARSE_QUOTED_STRING", value, (STRING_VIEW("Hello \\\"name\\\" \\n and welcome!")));
    ASSERT_EQUAL_RANGE("PARSE_QUOTED_STRING", string, (STRING_VIEW(", 1, 2")));
}

void test_parse_json_array() {
    auto string = STRING_VIEW("[ 1, 2 ,3 ], 4");
    auto value = PARSE_JSON_ARRAY(string);
    ASSERT_EQUAL_RANGE("PARSE_JSON_ARRAY", value, (STRING_VIEW("[ 1, 2 ,3 ]")));
    ASSERT_EQUAL_RANGE("PARSE_JSON_ARRAY", string, (STRING_VIEW(", 4")));
}

void test_parse_json_array_item_by_item() {
    auto string = STRING_VIEW("[ 1, 2 ,3 ]");
    auto expected = MAKE_DARRAY(IntArray, 1, 2, 3);
    auto actual = (IntArray){};
    for (auto item = parse_first_json_array_item(&string); !IS_EMPTY(item); item = parse_next_json_array_item(&string)) {
        auto optional_int = PARSE_INT(item);
        FOR_EACH(it, optional_int) {
            APPEND(actual, *it);
        }
    }
    ASSERT_EQUAL_RANGE("test_parse_json_array_item_by_item", actual, expected);
}

void test_for_each_json_array_item() {
    auto string = STRING_VIEW("[ 1, 2 ,3 ]");
    auto expected = MAKE_DARRAY(IntArray, 1, 2, 3);
    auto actual = (IntArray){};
    FOR_EACH_JSON_ARRAY_ITEM(item, string) {
        auto number = PARSE_INT_OR_EXIT(item);
        APPEND(actual, number);
    }
    ASSERT_EQUAL_RANGE("test_for_each_json_array_item", actual, expected);
}

void test_parse_json_object() {
    auto string = STRING_VIEW("{\"key\" : 1}, 2");
    auto value = parse_json_object(&string);
    ASSERT_EQUAL_RANGE("parse_json_object", value, (STRING_VIEW("{\"key\" : 1}")));
    ASSERT_EQUAL_RANGE("parse_json_object", string, (STRING_VIEW(", 2")));
}

void test_parse_json_object_item_by_item() {
    auto string = STRING_VIEW("{ \"a\" : 1 , \"b\": 2 }");
    auto expected_values = MAKE_DARRAY(IntArray, 1, 2);
    auto actual_values = (IntArray){};
    for (
        StringView k = parse_first_json_object_key(&string),
        v = parse_next_json_object_value(&string);
        !IS_EMPTY(k) && !IS_EMPTY(v);
        k = parse_next_json_object_key(&string),
        v = parse_next_json_object_value(&string)
    ) {
        auto optional_int = PARSE_INT(v);
        FOR_EACH(it, optional_int) {
            APPEND(actual_values, *it);
        }
    }
    ASSERT_EQUAL_RANGE("test_parse_json_object_item_by_item", actual_values, expected_values);
}

void test_for_each_json_object_item() {
    auto string = STRING_VIEW("{ \"a\" : 1 , \"b\": 2 }");
    auto expected_values = MAKE_DARRAY(IntArray, 1, 2);
    auto actual_values = (IntArray){};
    FOR_EACH_JSON_OBJECT_ITEM(k, v, string) {
        auto optional_int = PARSE_INT(v);
        FOR_EACH(it, optional_int) {
            APPEND(actual_values, *it);
        }
    }
    ASSERT_EQUAL_RANGE("test_for_each_json_object_item", actual_values, expected_values);
}

void test_parse_json_key() {
    auto string = STRING_VIEW("{ \"a\" : 1 , \"b\": 2 , \"c\": 3 }");
    auto value = parse_json_key(string, "b");
    ASSERT_EQUAL_RANGE("test_parse_json_key", value, (STRING_VIEW("2")));
}

void test_parse_json_key_int_or_exit() {
    auto string = STRING_VIEW("{ \"a\" : 1 , \"b\": 2 , \"c\": 3 }");
    auto value = parse_json_key_int_or_exit(string, "b");
    ASSERT_EQUAL_INT("test_parse_json_key_int_or_exit", value, 2);
}

void test_add_json_int() {
    auto actual = (JsonBuilder){};
    ADD_JSON_INT(actual, 1);
    auto expected = STRING_VIEW("1");
    ASSERT_EQUAL_RANGE("test_add_json_int", actual.string, expected);
}

void test_add_json_bool_true() {
    auto actual = (JsonBuilder){};
    ADD_JSON_BOOL(actual, true);
    auto expected = STRING_VIEW("true");
    ASSERT_EQUAL_RANGE("test_add_json_bool_true", actual.string, expected);
}

void test_add_json_bool_false() {
    auto actual = (JsonBuilder){};
    ADD_JSON_BOOL(actual, false);
    auto expected = STRING_VIEW("false");
    ASSERT_EQUAL_RANGE("test_add_json_bool_false", actual.string, expected);
}

void test_add_json_array_empty() {
    auto actual = (JsonBuilder){};
    ADD_JSON_ARRAY(actual) {
    }
    auto expected = STRING_VIEW("[]");
    ASSERT_EQUAL_RANGE("test_add_json_array_empty", actual.string, expected);
}

void test_add_json_array_single() {
    auto actual = (JsonBuilder){};
    ADD_JSON_ARRAY(actual) {
        ADD_JSON_INT(actual, 1);
    }
    auto expected = STRING_VIEW("[1]");
    ASSERT_EQUAL_RANGE("test_add_json_array_single", actual.string, expected);
}

void test_add_json_array_multiple() {
    auto actual = (JsonBuilder){};
    ADD_JSON_ARRAY(actual) {
        ADD_JSON_INT(actual, 1);
        ADD_JSON_INT(actual, 2);
    }
    auto expected = STRING_VIEW("[1,2]");
    ASSERT_EQUAL_CARMA_STRINGS("test_add_json_array_multiple", actual.string, expected);
}

void test_add_json_object_empty() {
    auto actual = (JsonBuilder){};
    ADD_JSON_OBJECT(actual) {
    }
    auto expected = STRING_VIEW("{}");
    ASSERT_EQUAL_CARMA_STRINGS("test_add_json_object_empty", actual.string, expected);
}

void test_add_json_object_single() {
    auto actual = (JsonBuilder){};
    ADD_JSON_OBJECT(actual) {
        ADD_JSON_KEY(actual, "a");
        ADD_JSON_INT(actual, 1);
    }
    auto expected = STRING_VIEW("{\"a\":1}");
    ASSERT_EQUAL_CARMA_STRINGS("test_add_json_object_single", actual.string, expected);
}

void test_add_json_object_multiple() {
    auto actual = (JsonBuilder){};
    ADD_JSON_OBJECT(actual) {
        ADD_JSON_KEY(actual, "a");
        ADD_JSON_INT(actual, 1);
        ADD_JSON_KEY(actual, "b");
        ADD_JSON_DOUBLE(actual, 2.0);
        ADD_JSON_KEY(actual, "c");
        ADD_JSON_CSTRING(actual, "d");
    }
    auto expected = STRING_VIEW("{\"a\":1,\"b\":2.000000,\"c\":\"d\"}");
    ASSERT_EQUAL_CARMA_STRINGS("test_add_json_object_multiple", actual.string, expected);
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

    test_sub_range0();
    test_sub_range1a();
    test_sub_range1b();
    test_sub_range1c();
    test_sub_range2a();
    test_sub_range2b();
    test_sub_range2c();

    test_for_each_backward2();

    test_for_each_backward_a();
    test_for_each_backward_b();
    test_for_each_backward_c();
    test_for_each_backward_d();

    test_for_min();
    test_for_max();

    test_are_equal();
    test_are_equal_not();
    test_are_equal_empty();
    test_are_equal_different_count();

    test_fill();

    test_copy0();
    test_copy1();
    test_copy2();

    test_copy_inplace();
    test_copy_backward_inplace();

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

    test_erase_back();
    test_erase_front();

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

    test_erase_many_ordered1();
    test_erase_many_ordered2();
    test_erase_many_ordered3();

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

    test_insert_range0a();
    test_insert_range0b();
    test_insert_range1a();
    test_insert_range1b();

    test_optional();

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

    test_parse_int();
    test_parse_int_or_exit();
    test_parse_int_as_string();
    test_parse_line();
    test_parse_whitespace();
    test_parse_quoted_string();

    test_parse_json_array();
    test_parse_json_object();
    test_parse_json_array_item_by_item();
    test_for_each_json_array_item();
    test_parse_json_object_item_by_item();
    test_for_each_json_object_item();
    test_parse_json_key();
    test_parse_json_key_int_or_exit();

    test_add_json_int();
    test_add_json_bool_true();
    test_add_json_bool_false();

    test_add_json_array_empty();
    test_add_json_array_single();
    test_add_json_array_multiple();

    test_add_json_object_empty();
    test_add_json_object_single();
    test_add_json_object_multiple();

    CHECK_INTERNAL(true);
    CHECK_EXTERNAL(true, "Some external error");
    (void)CHECK_INTERNAL_VALUE(1, true);
    (void)CHECK_EXTERNAL_VALUE(1, true, "Some external error");

    summarize_tests();
    
    return 0;
}
