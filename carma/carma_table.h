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

#define CARMA_HASH_KEYS(keys) \
    carma_hash_bytes(CARMA_HASH_INIT, (const char*)(BEGIN_POINTER(keys)), COUNT_BYTES(keys))

#define CARMA_GET_FIRST_KEY_ITEM(key, table) \
    ((table).data + CARMA_HASH_KEY(key) % (table).count)

#define CARMA_GET_FIRST_KEYS_ITEM(keys, table) \
    ((table).data + CARMA_HASH_KEYS(keys) % (table).count)
    
#define GET_KEY_VALUE(k, _value, table) do { \
    if (IS_EMPTY(table)) \
        break; \
    auto _key = (k); \
    auto _item = CARMA_GET_FIRST_KEY_ITEM(_key, table); \
    if (_item->occupied && _item->key == _key) { \
        (_value) = _item->value; \
    } \
} while (0)

#define GET_KEYS_VALUE(_keys, _value, table) do { \
    if (IS_EMPTY(table)) \
        break; \
    auto _item = CARMA_GET_FIRST_KEYS_ITEM(_keys, table); \
    if (_item->occupied && ARE_EQUAL(_item->keys, (_keys))) { \
        (_value) = _item->value; \
    } \
} while (0)

////////////////////////////////////////////////////////////////////////////////
// ADD DATA TO TABLE

#define CARMA_FIND_FREE_INDEX_FOR_KEY(table, k, _it) do { \
    _it = CARMA_GET_FIRST_KEY_ITEM((k), (table)); \
    if (!_it->occupied) { \
    } \
    else if (_it->key == (k)) { \
    } \
    else { \
        (_it) = NULL; \
    } \
} while (0)

#define CARMA_FIND_FREE_INDEX_FOR_KEYS(table, _keys, _it) do { \
    _it = CARMA_GET_FIRST_KEYS_ITEM((_keys), (table)); \
    if (!_it->occupied) { \
    } \
    else if (ARE_EQUAL(_it->keys, _keys)) { \
    } \
    else { \
        (_it) = NULL; \
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
    FOR_EACH_TABLE(_old_item, (table)) { \
        auto _new_item = new_table.data; \
        CARMA_FIND_FREE_INDEX_FOR_KEY((new_table), _old_item->key, _new_item); \
        assert(_new_item != NULL); \
        *_new_item = *_old_item; \
    } \
    FREE_TABLE(table); \
    table = new_table; \
} while (0)

#define CARMA_DOUBLE_TABLE_CAPACITY_KEYS(table) do { \
    auto new_capacity = 2 * (table).capacity; \
    auto new_table = table; \
    INIT_TABLE(new_table, new_capacity); \
    FOR_EACH_TABLE(_old_item, (table)) { \
        auto _new_item = new_table.data; \
        CARMA_FIND_FREE_INDEX_FOR_KEYS((new_table), _old_item->keys, _new_item); \
        assert(_new_item != NULL); \
        *_new_item = *_old_item; \
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
    auto _k = (k); \
    auto _item = (table).data; \
    CARMA_FIND_FREE_INDEX_FOR_KEY((table), _k, _item); \
    while (_item == NULL) { \
        CARMA_DOUBLE_TABLE_CAPACITY_KEY(table); \
        CARMA_FIND_FREE_INDEX_FOR_KEY((table), _k, _item); \
    } \
    _item->key = _k; \
    _item->value = (v); \
    _item->occupied = (true); \
} while (0)

#define SET_KEYS_VALUE(k, v, table) do { \
    CARMA_HANDLE_EMPTY_TABLE(table); \
    auto _k = (k); \
    auto _item = (table).data; \
    CARMA_FIND_FREE_INDEX_FOR_KEYS((table), _k, _item); \
    while (_item == NULL) { \
        CARMA_DOUBLE_TABLE_CAPACITY_KEYS(table); \
        CARMA_FIND_FREE_INDEX_FOR_KEYS((table), _k, _item); \
    } \
    _item->keys = _k; \
    _item->value = (v); \
    _item->occupied = (true); \
} while (0)
