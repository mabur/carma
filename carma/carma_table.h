#pragma once

#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>

#include "carma.h"

////////////////////////////////////////////////////////////////////////////////
// FIND DATA IN TABLE

size_t _update_hash(size_t hash, char data) {
    return ((hash << 5) + hash) + data;
}

size_t _hash_primitive(const char* data, size_t count) {
    size_t hash = 5381;
    for (size_t i = 0; i < count; ++i) {
        hash = _update_hash(hash, data[i]);
    }
    return hash;
}

#define _HASH_PRIMITIVE(key) _hash_primitive((const char*)&(key), sizeof(key))

#define _FIND_BASE_INDEX(table, key) \
    (_hash_primitive((const char*)&(key), sizeof(key)) % (table).count)

#define FOR_STATE(name, value) \
    for (typeof(value) (name) = (value), (name##count) = 0; !(name##count); ++(name##count))

// TODO: extend to interval
#define FIND_KEY(k, value_it, table) \
    if (!IS_EMPTY(table)) \
    FOR_STATE(_k, (k)) \
    FOR_STATE(_i, _FIND_BASE_INDEX((table), _k)) \
    FOR_STATE(value_it, &(table).data[_i].value) \
    if ((table).data[_i].occupied) \
    if ((table).data[_i].key == k)

////////////////////////////////////////////////////////////////////////////////
// ADD DATA TO TABLE

// TODO: extend to interval
#define _FIND_FREE_INDEX(table, k, index) do { \
    auto _lvalue_key = (k); \
    auto _hash = _HASH_PRIMITIVE(_lvalue_key); \
    (index) = (_hash) % (table).count; \
    if (!(table).data[index].occupied) { \
    } \
    else if ((table).data[index].key == _lvalue_key) { \
    } \
    else { \
        (index) = SIZE_MAX; \
    } \
} while (0)

#define CLEAR_TABLE(table) FOR_EACH(item, (table)) item->occupied = false;

#define _DOUBLE_TABLE_CAPACITY(table) do { \
    auto new_capacity = 2 * (table).capacity; \
    auto new_table = table; \
    INIT_DARRAY(new_table, new_capacity, new_capacity); \
    CLEAR_TABLE(new_table); \
    FOR_EACH(_item, (table)) { \
        if (!_item->occupied) continue; \
        size_t _inner_index; \
        _FIND_FREE_INDEX((new_table), _item->key, _inner_index); \
        assert(_inner_index != SIZE_MAX); \
        new_table.data[_inner_index] = *_item; \
    } \
    FREE_DARRAY(table); \
    table = new_table; \
} while (0)

#define SET_KEY_VALUE(k, v, table) do { \
    if (IS_EMPTY(table)) { \
        INIT_DARRAY((table), 1, 1); \
        CLEAR_TABLE(table); \
    } \
    size_t index; \
    _FIND_FREE_INDEX((table), (k), index); \
    while (index == SIZE_MAX) { \
        _DOUBLE_TABLE_CAPACITY(table); \
        _FIND_FREE_INDEX((table), (k), index); \
    } \
    (table).data[index].key = (k); \
    (table).data[index].value = (v); \
    (table).data[index].occupied = (true); \
} while (0)
