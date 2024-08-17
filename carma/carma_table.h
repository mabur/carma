#pragma once

#include <limits.h>
#include <stdbool.h>
#include <stddef.h>

#include "carma.h"

#define FOR_KEY_VALUE(key, value, table) \
    auto key = BEGIN_POINTER(table.keys); \
    auto value = BEGIN_POINTER(table.values); \
    for (auto _occupied = BEGIN_POINTER(table.occupied); \
        _occupied != END_POINTER(table.occupied); ++key, ++value, ++_occupied) \
        if (*_occupied) 

#define INIT_TABLE(table, capacity) do { \
    INIT_DARRAY((table).keys, (capacity), (capacity)); \
    INIT_DARRAY((table).values, (capacity), (capacity)); \
    INIT_DARRAY((table).occupied, (capacity), (capacity)); \
    FILL((table).occupied, false); \
} while (0)

#define FREE_TABLE(table) do { \
    FREE_DARRAY((table).keys); \
    FREE_DARRAY((table).values); \
    FREE_DARRAY((table).occupied); \
} while (0)

#define findBaseIndex(table, key) ((table).hash(key) % (table).keys.count)

// TODO: extend to interval
#define findFreeIndex(table, key, index) do { \
    if (!table.occupied.data[index]) { \
    } \
    else if (table.keys.data[index] == key) { \
    } \
    else { \
        index = SIZE_MAX; \
    } \
} while (0)

// How to return value in macro?
/*
inline
bool CONTAINS(IntTable table, int key) {
    // TODO: extend to interval
    auto base_index = findBaseIndex(table, key);
    // If not occupied:
    if (!table.occupied.data[base_index]) {
        return false;
    }
    // If occupied by same key:
    if (table.keys.data[base_index] == key) {
        return true;
    }
    return false;
}
*/

// How to handle missing key? As pre-condition? Like FOR_MAX?
// TODO: extend to interval
#define FOR_KEY(key, value_it, table) \
    auto _i = findBaseIndex((table), (key)); \
    auto value_it = (table).values.data + _i; \
    if (table.occupied.data[_i] && table.keys.data[_i] == key)

#define DOUBLE_TABLE_CAPACITY(table) do { \
    auto old_capacity = (table).keys.capacity; \
    auto new_capacity = old_capacity ? 2 * old_capacity : 1; \
    auto new_table = table; \
    INIT_TABLE(new_table, new_capacity); \
    FOR_KEY_VALUE(inner_key, inner_value, (table)) { \
        auto inner_base_index = findBaseIndex((table), *inner_key); \
        auto free_index_inner = inner_base_index; \
        findFreeIndex((table), *inner_key, free_index_inner); \
        new_table.keys.data[free_index_inner] = *inner_key; \
        new_table.values.data[free_index_inner] = *inner_value; \
        new_table.occupied.data[free_index_inner] = true; \
    } \
    FREE_TABLE(table); \
    table = new_table; \
} while (0)

#define SET_KEY_VALUE(table, key, value) do { \
    if (IS_EMPTY(table.keys)) { \
        DOUBLE_TABLE_CAPACITY(table); \
    } \
    auto base_index = findBaseIndex((table), (key)); \
    auto index = base_index; \
    findFreeIndex((table), (key), index); \
    while (index == SIZE_MAX) { \
        DOUBLE_TABLE_CAPACITY(table); \
        index = base_index;                   \
        findFreeIndex((table), (key), index); \
    } \
    (table).keys.data[index] = (key); \
    (table).values.data[index] = (value); \
    (table).occupied.data[index] = true; \
} while (0)
