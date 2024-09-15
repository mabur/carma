#pragma once

#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>

#include "carma.h"

////////////////////////////////////////////////////////////////////////////////
// FIND DATA IN TABLE

size_t _hash_byte(size_t hash, char data) {
    return ((hash << 5) + hash) + data;
}

size_t _hash_bytes(size_t hash, const char* data, size_t count) {
    for (size_t i = 0; i < count; ++i) {
        hash = _hash_byte(hash, data[i]);
    }
    return hash;
}

#define _HASH_INIT 5381

#define _BYTES_RANGE(key) (const char*)&(key), sizeof(key)

#define _HASH_PRIMITIVE(key) _hash_bytes(_HASH_INIT, _BYTES_RANGE(key))

#define _HASH_2PRIMITIVES(k0, k1) \
    _hash_bytes(_hash_bytes(_HASH_INIT, _BYTES_RANGE(k0)), _BYTES_RANGE(k1))

#define FOR_STATE(name, value) \
    for (typeof(value) (name) = (value), (name##count) = 0; !(name##count); ++(name##count))

// TODO: extend to interval
#define FIND_KEY(k, value_it, table) \
    if (!IS_EMPTY(table)) \
    FOR_STATE(_k, (k)) \
    FOR_STATE(_i, _HASH_PRIMITIVE(_k) % (table).count) \
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

// TODO: extend to interval
#define _FIND_FREE_INDEX2(table, k0, k1, index) do { \
    auto _lvalue_k0 = (k0); \
    auto _lvalue_k1 = (k1); \
    auto _hash = _HASH_2PRIMITIVES(_lvalue_k0, _lvalue_k1); \
    (index) = (_hash) % (table).count; \
    if (!(table).data[index].occupied) { \
    } \
    else if ((table).data[index].key0 == _lvalue_k0 && (table).data[index].key1 == _lvalue_k1) { \
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

#define _DOUBLE_TABLE_CAPACITY2(table) do { \
    auto new_capacity = 2 * (table).capacity; \
    auto new_table = table; \
    INIT_DARRAY(new_table, new_capacity, new_capacity); \
    CLEAR_TABLE(new_table); \
    FOR_EACH(_item, (table)) { \
        if (!_item->occupied) continue; \
        size_t _inner_index; \
        _FIND_FREE_INDEX2((new_table), _item->key0, _item->key1, _inner_index); \
        assert(_inner_index != SIZE_MAX); \
        new_table.data[_inner_index] = *_item; \
    } \
    FREE_DARRAY(table); \
    table = new_table; \
} while (0)

#define _HANDLE_EMPTY_TABLE(table) \
    if (IS_EMPTY(table)) { \
        INIT_DARRAY((table), 1, 1); \
        CLEAR_TABLE(table); \
    }

#define SET_KEY_VALUE(k, v, table) do { \
    _HANDLE_EMPTY_TABLE(table); \
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

#define SET_2KEYS_VALUE(k0, k1, v, table) do { \
    _HANDLE_EMPTY_TABLE(table); \
    size_t index; \
    _FIND_FREE_INDEX2((table), (k0), (k1), index); \
    while (index == SIZE_MAX) { \
        _DOUBLE_TABLE_CAPACITY2(table); \
        _FIND_FREE_INDEX2((table), (k0), (k1), index); \
    } \
    (table).data[index].key0 = (k0); \
    (table).data[index].key1 = (k1); \
    (table).data[index].value = (v); \
    (table).data[index].occupied = (true); \
} while (0)
