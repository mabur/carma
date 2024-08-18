#pragma once

#include <limits.h>
#include <stdbool.h>
#include <stddef.h>

#include "carma.h"

////////////////////////////////////////////////////////////////////////////////
// INIT AND FREE TABLE

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

////////////////////////////////////////////////////////////////////////////////
// FIND DATA IN TABLE

#define FIND_BASE_INDEX(table, key) ((table).hash(key) % (table).keys.count)

#define FOR_EACH_KEY_VALUE(key, value, table) \
    auto key = BEGIN_POINTER(table.keys); \
    auto value = BEGIN_POINTER(table.values); \
    for (auto _occupied = BEGIN_POINTER(table.occupied); \
        _occupied != END_POINTER(table.occupied); ++key, ++value, ++_occupied) \
        if (*_occupied)

#define FOR_STATE(name, value) \
    for (typeof(value) (name) = (value), (name##count) = 0; !(name##count); ++(name##count))

// TODO: extend to interval
// TODO: allow custom comparison of keys
#define FIND_KEY(key, value_it, table) \
    if (!IS_EMPTY((table).keys)) \
    FOR_STATE(_i, FIND_BASE_INDEX((table), (key))) \
    FOR_STATE(value_it, (table).values.data + _i) \
    if (table.occupied.data[_i]) \
    if (table.keys.data[_i] == key)

////////////////////////////////////////////////////////////////////////////////
// ADD DATA TO TABLE

// TODO: extend to interval
// TODO: allow custom comparison of keys
#define FIND_FREE_INDEX(table, key, index) do { \
    if (!table.occupied.data[index]) { \
    } \
    else if (table.keys.data[index] == key) { \
    } \
    else { \
        index = SIZE_MAX; \
    } \
} while (0)

#define DOUBLE_TABLE_CAPACITY(table) do { \
    auto old_capacity = (table).keys.capacity; \
    auto new_capacity = old_capacity ? 2 * old_capacity : 1; \
    auto new_table = table; \
    INIT_TABLE(new_table, new_capacity); \
    FOR_EACH_KEY_VALUE(inner_key, inner_value, (table)) { \
        auto inner_base_index = FIND_BASE_INDEX((table), *inner_key); \
        auto free_index_inner = inner_base_index; \
        FIND_FREE_INDEX((table), *inner_key, free_index_inner); \
        new_table.keys.data[free_index_inner] = *inner_key; \
        new_table.values.data[free_index_inner] = *inner_value; \
        new_table.occupied.data[free_index_inner] = true; \
    } \
    FREE_TABLE(table); \
    table = new_table; \
} while (0)

#define SET_KEY_VALUE(key, value, table) do { \
    if (IS_EMPTY(table.keys)) { \
        DOUBLE_TABLE_CAPACITY(table); \
    } \
    auto base_index = FIND_BASE_INDEX((table), (key)); \
    auto index = base_index; \
    FIND_FREE_INDEX((table), (key), index); \
    while (index == SIZE_MAX) { \
        DOUBLE_TABLE_CAPACITY(table); \
        index = base_index;                   \
        FIND_FREE_INDEX((table), (key), index); \
    } \
    (table).keys.data[index] = (key); \
    (table).values.data[index] = (value); \
    (table).occupied.data[index] = true; \
} while (0)
