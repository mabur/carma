#pragma once

#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>

#include "carma.h"

////////////////////////////////////////////////////////////////////////////////
// FIND DATA IN TABLE

size_t hash_primitive(const char* data, size_t count) {
    size_t hash = 5381;
    for (size_t i = 0; i < count; ++i) {
        hash = ((hash << 5) + hash) + data[i];
    }
    return hash;
}

#define FIND_BASE_INDEX2(table, key) \
    (hash_primitive((const char*)&(key), sizeof(key)) % (table).count)

#define FOR_STATE2(name, value) \
    for (typeof(value) (name) = (value), (name##count) = 0; !(name##count); ++(name##count))

// TODO: extend to interval
#define FIND_KEY2(key, value_it, table) \
    if (!IS_EMPTY((table).keys)) \
    FOR_STATE2(_i, FIND_BASE_INDEX2((table), (key))) \
    FOR_STATE2(value_it, (table).values.data + _i) \
    if (table.occupied.data[_i]) \
    if (table.strategy.equals(table.keys.data[_i], key))

////////////////////////////////////////////////////////////////////////////////
// ADD DATA TO TABLE

// TODO: extend to interval
#define FIND_FREE_INDEX2(table, k, index) do { \
    if (!(table).data[index].occupied) { \
    } \
    else if ((table).data[index].key == (k)) { \
    } \
    else { \
        (index) = SIZE_MAX; \
    } \
} while (0)

// TODO: are table and new_table swapped in implementation?
#define DOUBLE_TABLE_CAPACITY2(table) do { \
    auto old_capacity = (table).capacity; \
    auto new_capacity = old_capacity ? 2 * old_capacity : 1; \
    auto new_table = table; \
    INIT_DARRAY(new_table, new_capacity, new_capacity); \
    FOR_EACH(_item, new_table) { \
        _item->occupied = false; \
    } \
    FOR_EACH(_item, (table)) { \
        auto _key = _item->key; \
        auto inner_base_index = FIND_BASE_INDEX2((table), _key); \
        auto free_index_inner = inner_base_index; \
        FIND_FREE_INDEX2((table), _key, free_index_inner); \
        assert(free_index_inner != SIZE_MAX); \
        new_table.data[free_index_inner] = *_item; \
    } \
    FREE_DARRAY(table); \
    table = new_table; \
} while (0)

#define SET_KEY_VALUE2(k, v, table) do { \
    if (IS_EMPTY(table)) { \
        DOUBLE_TABLE_CAPACITY2(table); \
    } \
    auto _lvalue_key = k; \
    auto base_index = FIND_BASE_INDEX2((table), _lvalue_key); \
    auto index = base_index; \
    FIND_FREE_INDEX2((table), (k), index); \
    while (index == SIZE_MAX) { \
        DOUBLE_TABLE_CAPACITY2(table); \
        index = base_index;                   \
        FIND_FREE_INDEX2((table), (k), index); \
    } \
    (table).data[index].key = (k); \
    (table).data[index].value = (v); \
    (table).data[index].occupied = (true); \
} while (0)
