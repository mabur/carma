#pragma once

#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>

#include "carma.h"

////////////////////////////////////////////////////////////////////////////////
// FIND DATA IN TABLE

static inline
size_t carma_hash_byte(size_t hash, char data) {
    return ((hash << 5) + hash) + data;
}

static inline
size_t carma_hash_bytes(size_t hash, const char* data, size_t count) {
    for (size_t i = 0; i < count; ++i) {
        hash = carma_hash_byte(hash, data[i]);
    }
    return hash;
}

#define CARMA_HASH_INIT 5381

#define CARMA_BYTES_RANGE(key) (const char*)&(key), sizeof(key)

#define CARMA_HASH_KEY(key) carma_hash_bytes(CARMA_HASH_INIT, CARMA_BYTES_RANGE(key))

#define CARMA_HASH_2_KEYS(k0, k1) \
    carma_hash_bytes(carma_hash_bytes(CARMA_HASH_INIT, CARMA_BYTES_RANGE(k0)), CARMA_BYTES_RANGE(k1))

#define CARMA_HASH_KEYS(keys) \
    carma_hash_bytes(CARMA_HASH_INIT, (const char*)(BEGIN_POINTER(keys)), COUNT_BYTES(keys))
    
#define GET_KEY_VALUE(k, _value, table) do { \
    if (IS_EMPTY(table)) \
        break; \
    auto _key = (k); \
    auto _index = CARMA_HASH_KEY(_key) % (table).count; \
    auto _item = (table).data + _index; \
    if (_item->occupied && _item->key == _key) { \
        (_value) = _item->value; \
    } \
} while(0)

#define GET_2_KEYS_VALUE(k0, k1, _value, table) do { \
    if (IS_EMPTY(table)) \
        break; \
    auto _key0 = (k0); \
    auto _key1 = (k1); \
    auto _index = CARMA_HASH_2_KEYS(_key0, _key1) % (table).count; \
    auto _item = (table).data + _index; \
    if (_item->occupied && _item->key0 == _key0 && _item->key1 == _key1) { \
        (_value) = _item->value; \
    } \
} while(0)

#define GET_KEYS_VALUE(_keys, _value, table) do { \
    if (IS_EMPTY(table)) \
        break; \
    auto _index = CARMA_HASH_KEYS(_keys) % (table).count; \
    auto _item = (table).data + _index; \
    if (_item->occupied && ARE_EQUAL(_item->keys, (_keys))) { \
        (_value) = _item->value; \
    } \
} while(0)

////////////////////////////////////////////////////////////////////////////////
// ADD DATA TO TABLE

#define CARMA_FIND_FREE_INDEX_FOR_KEY(table, k, index) do { \
    auto _lvalue_key = (k); \
    auto _hash = CARMA_HASH_KEY(_lvalue_key); \
    (index) = (_hash) % (table).count; \
    if (!(table).data[index].occupied) { \
    } \
    else if ((table).data[index].key == _lvalue_key) { \
    } \
    else { \
        (index) = SIZE_MAX; \
    } \
} while (0)

#define CARMA_FIND_FREE_INDEX_FOR_2_KEYS(table, k0, k1, index) do { \
    auto _lvalue_k0 = (k0); \
    auto _lvalue_k1 = (k1); \
    auto _hash = CARMA_HASH_2_KEYS(_lvalue_k0, _lvalue_k1); \
    (index) = (_hash) % (table).count; \
    if (!(table).data[index].occupied) { \
    } \
    else if ((table).data[index].key0 == _lvalue_k0 && (table).data[index].key1 == _lvalue_k1) { \
    } \
    else { \
        (index) = SIZE_MAX; \
    } \
} while (0)

#define CARMA_FIND_FREE_INDEX_FOR_KEYS(table, _keys, index) do { \
    auto _hash = CARMA_HASH_KEYS(_keys); \
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
    
#define CARMA_DOUBLE_TABLE_CAPACITY_KEY(table) do { \
    auto new_capacity = 2 * (table).capacity; \
    auto new_table = table; \
    INIT_TABLE(new_table, new_capacity); \
    FOR_EACH_TABLE(_item, (table)) { \
        size_t _inner_index; \
        CARMA_FIND_FREE_INDEX_FOR_KEY((new_table), _item->key, _inner_index); \
        assert(_inner_index != SIZE_MAX); \
        new_table.data[_inner_index] = *_item; \
    } \
    FREE_TABLE(table); \
    table = new_table; \
} while (0)

#define CARMA_DOUBLE_TABLE_CAPACITY_2_KEYS(table) do { \
    auto new_capacity = 2 * (table).capacity; \
    auto new_table = table; \
    INIT_TABLE(new_table, new_capacity); \
    FOR_EACH_TABLE(_item, (table)) { \
        size_t _inner_index; \
        CARMA_FIND_FREE_INDEX_FOR_2_KEYS((new_table), _item->key0, _item->key1, _inner_index); \
        assert(_inner_index != SIZE_MAX); \
        new_table.data[_inner_index] = *_item; \
    } \
    FREE_TABLE(table); \
    table = new_table; \
} while (0)

#define CARMA_DOUBLE_TABLE_CAPACITY_KEYS(table) do { \
    auto new_capacity = 2 * (table).capacity; \
    auto new_table = table; \
    INIT_TABLE(new_table, new_capacity); \
    FOR_EACH_TABLE(_item, (table)) { \
        size_t _inner_index; \
        CARMA_FIND_FREE_INDEX_FOR_KEYS((new_table), _item->keys, _inner_index); \
        assert(_inner_index != SIZE_MAX); \
        new_table.data[_inner_index] = *_item; \
    } \
    FREE_TABLE(table); \
    table = new_table; \
} while (0)

#define CARMA_HANDLE_EMPTY_TABLE(table) \
    if (IS_EMPTY(table)) { \
        INIT_TABLE(table, 1); \
    }

#define SET_KEY_VALUE(k, v, table) do { \
    CARMA_HANDLE_EMPTY_TABLE(table); \
    size_t index; \
    CARMA_FIND_FREE_INDEX_FOR_KEY((table), (k), index); \
    while (index == SIZE_MAX) { \
        CARMA_DOUBLE_TABLE_CAPACITY_KEY(table); \
        CARMA_FIND_FREE_INDEX_FOR_KEY((table), (k), index); \
    } \
    (table).data[index].key = (k); \
    (table).data[index].value = (v); \
    (table).data[index].occupied = (true); \
} while (0)

#define SET_2_KEYS_VALUE(k0, k1, v, table) do { \
    CARMA_HANDLE_EMPTY_TABLE(table); \
    size_t index; \
    CARMA_FIND_FREE_INDEX_FOR_2_KEYS((table), (k0), (k1), index); \
    while (index == SIZE_MAX) { \
        CARMA_DOUBLE_TABLE_CAPACITY_2_KEYS(table); \
        CARMA_FIND_FREE_INDEX_FOR_2_KEYS((table), (k0), (k1), index); \
    } \
    (table).data[index].key0 = (k0); \
    (table).data[index].key1 = (k1); \
    (table).data[index].value = (v); \
    (table).data[index].occupied = (true); \
} while (0)

#define SET_KEYS_VALUE(k, v, table) do { \
    CARMA_HANDLE_EMPTY_TABLE(table); \
    size_t index; \
    CARMA_FIND_FREE_INDEX_FOR_KEYS((table), (k), index); \
    while (index == SIZE_MAX) { \
        CARMA_DOUBLE_TABLE_CAPACITY_KEYS(table); \
        CARMA_FIND_FREE_INDEX_FOR_KEYS((table), (k), index); \
    } \
    (table).data[index].keys = (k); \
    (table).data[index].value = (v); \
    (table).data[index].occupied = (true); \
} while (0)
