#pragma once

#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>

#include "carma.h"

////////////////////////////////////////////////////////////////////////////////
// FIND DATA IN TABLE

static inline
size_t _hash_byte(size_t hash, char data) {
    return ((hash << 5) + hash) + data;
}

static inline
size_t _hash_bytes(size_t hash, const char* data, size_t count) {
    for (size_t i = 0; i < count; ++i) {
        hash = _hash_byte(hash, data[i]);
    }
    return hash;
}

#define _HASH_INIT 5381

#define _BYTES_RANGE(key) (const char*)&(key), sizeof(key)

#define _HASH_KEY(key) _hash_bytes(_HASH_INIT, _BYTES_RANGE(key))

#define _HASH_2_KEYS(k0, k1) \
    _hash_bytes(_hash_bytes(_HASH_INIT, _BYTES_RANGE(k0)), _BYTES_RANGE(k1))

#define _HASH_KEYS(keys) \
    _hash_bytes(_HASH_INIT, (const char*)(BEGIN_POINTER(keys)), COUNT_BYTES(keys))
    
#define FOR_STATE(name, value) \
    for (typeof(value) (name) = (value), (name##count) = 0; !(name##count); ++(name##count))

// TODO: extend to interval
#define FIND_KEY(k, value_it, table) \
    if (!IS_EMPTY(table)) \
    FOR_STATE(_k, (k)) \
    FOR_STATE(_i, _HASH_KEY(_k) % (table).count) \
    FOR_STATE(value_it, &(table).data[_i].value) \
    if ((table).data[_i].occupied) \
    if ((table).data[_i].key == k)


// TODO: extend to interval
#define FIND_2_KEYS(k0, k1, value_it, table) \
    if (!IS_EMPTY(table)) \
    FOR_STATE(_k0, (k0)) \
    FOR_STATE(_k1, (k1)) \
    FOR_STATE(_i, _HASH_2_KEYS(_k0, _k1) % (table).count) \
    FOR_STATE(value_it, &(table).data[_i].value) \
    if ((table).data[_i].occupied) \
    if ((table).data[_i].key0 == k0) \
    if ((table).data[_i].key1 == k1)

// TODO: extend to interval
#define FIND_KEYS(k, value_it, table) \
    if (!IS_EMPTY(table)) \
    FOR_STATE(_i, _HASH_KEYS(k) % (table).count) \
    FOR_STATE(value_it, &(table).data[_i].value) \
    if ((table).data[_i].occupied) \
    if (ARE_EQUAL(table.data[_i].keys, k))
    
////////////////////////////////////////////////////////////////////////////////
// ADD DATA TO TABLE

// TODO: extend to interval
#define _FIND_FREE_INDEX_FOR_KEY(table, k, index) do { \
    auto _lvalue_key = (k); \
    auto _hash = _HASH_KEY(_lvalue_key); \
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
#define _FIND_FREE_INDEX_FOR_2_KEYS(table, k0, k1, index) do { \
    auto _lvalue_k0 = (k0); \
    auto _lvalue_k1 = (k1); \
    auto _hash = _HASH_2_KEYS(_lvalue_k0, _lvalue_k1); \
    (index) = (_hash) % (table).count; \
    if (!(table).data[index].occupied) { \
    } \
    else if ((table).data[index].key0 == _lvalue_k0 && (table).data[index].key1 == _lvalue_k1) { \
    } \
    else { \
        (index) = SIZE_MAX; \
    } \
} while (0)

// TODO: extend to interval
#define _FIND_FREE_INDEX_FOR_KEYS(table, _keys, index) do { \
    auto _hash = _HASH_KEYS(_keys); \
    (index) = (_hash) % (table).count; \
    if (!(table).data[index].occupied) { \
    } \
    else if (ARE_EQUAL((table).data[index].keys, _keys)) { \
    } \
    else { \
        (index) = SIZE_MAX; \
    } \
} while (0)

#define CLEAR_TABLE(table) FOR_EACH(item, (table)) item->occupied = false;

static inline
bool _is_power_of_two(size_t n) {
    // A power of two is greater than 0 and has only one bit set.
    // If n is a power of two then n - 1 will have all bits set,
    // below the single bit set in n, and n & (n - 1) will be 0.
    return (n != 0) && ((n & (n - 1)) == 0);
}

#define INIT_TABLE(table, capacity) do { \
    assert(_is_power_of_two(capacity)); \
    INIT_DARRAY(table, capacity, capacity); \
    CLEAR_TABLE(table); \
} while (0)

#define FREE_TABLE(table) FREE_DARRAY(table)

#define FOR_EACH_TABLE(iterator, table) \
    FOR_EACH(iterator, (table)) if ((iterator)->occupied)
    
#define _DOUBLE_TABLE_CAPACITY_KEY(table) do { \
    auto new_capacity = 2 * (table).capacity; \
    auto new_table = table; \
    INIT_TABLE(new_table, new_capacity); \
    FOR_EACH_TABLE(_item, (table)) { \
        size_t _inner_index; \
        _FIND_FREE_INDEX_FOR_KEY((new_table), _item->key, _inner_index); \
        assert(_inner_index != SIZE_MAX); \
        new_table.data[_inner_index] = *_item; \
    } \
    FREE_TABLE(table); \
    table = new_table; \
} while (0)

#define _DOUBLE_TABLE_CAPACITY_2_KEYS(table) do { \
    auto new_capacity = 2 * (table).capacity; \
    auto new_table = table; \
    INIT_TABLE(new_table, new_capacity); \
    FOR_EACH_TABLE(_item, (table)) { \
        size_t _inner_index; \
        _FIND_FREE_INDEX_FOR_2_KEYS((new_table), _item->key0, _item->key1, _inner_index); \
        assert(_inner_index != SIZE_MAX); \
        new_table.data[_inner_index] = *_item; \
    } \
    FREE_TABLE(table); \
    table = new_table; \
} while (0)

#define _DOUBLE_TABLE_CAPACITY_KEYS(table) do { \
    auto new_capacity = 2 * (table).capacity; \
    auto new_table = table; \
    INIT_TABLE(new_table, new_capacity); \
    FOR_EACH_TABLE(_item, (table)) { \
        size_t _inner_index; \
        _FIND_FREE_INDEX_FOR_KEYS((new_table), _item->keys, _inner_index); \
        assert(_inner_index != SIZE_MAX); \
        new_table.data[_inner_index] = *_item; \
    } \
    FREE_TABLE(table); \
    table = new_table; \
} while (0)

#define _HANDLE_EMPTY_TABLE(table) \
    if (IS_EMPTY(table)) { \
        INIT_TABLE(table, 1); \
    }

#define SET_KEY_VALUE(k, v, table) do { \
    _HANDLE_EMPTY_TABLE(table); \
    size_t index; \
    _FIND_FREE_INDEX_FOR_KEY((table), (k), index); \
    while (index == SIZE_MAX) { \
        _DOUBLE_TABLE_CAPACITY_KEY(table); \
        _FIND_FREE_INDEX_FOR_KEY((table), (k), index); \
    } \
    (table).data[index].key = (k); \
    (table).data[index].value = (v); \
    (table).data[index].occupied = (true); \
} while (0)

#define SET_2_KEYS_VALUE(k0, k1, v, table) do { \
    _HANDLE_EMPTY_TABLE(table); \
    size_t index; \
    _FIND_FREE_INDEX_FOR_2_KEYS((table), (k0), (k1), index); \
    while (index == SIZE_MAX) { \
        _DOUBLE_TABLE_CAPACITY_2_KEYS(table); \
        _FIND_FREE_INDEX_FOR_2_KEYS((table), (k0), (k1), index); \
    } \
    (table).data[index].key0 = (k0); \
    (table).data[index].key1 = (k1); \
    (table).data[index].value = (v); \
    (table).data[index].occupied = (true); \
} while (0)

#define SET_KEYS_VALUE(k, v, table) do { \
    _HANDLE_EMPTY_TABLE(table); \
    size_t index; \
    _FIND_FREE_INDEX_FOR_KEYS((table), (k), index); \
    while (index == SIZE_MAX) { \
        _DOUBLE_TABLE_CAPACITY_KEYS(table); \
        _FIND_FREE_INDEX_FOR_KEYS((table), (k), index); \
    } \
    (table).data[index].keys = (k); \
    (table).data[index].value = (v); \
    (table).data[index].occupied = (true); \
} while (0)
