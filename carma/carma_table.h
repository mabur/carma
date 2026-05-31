#pragma once

#include <limits.h>
#include <stdbool.h>
#include <stddef.h>

#include "carma.h"

////////////////////////////////////////////////////////////////////////////////
// HASH FUNCTIONS

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

#define CARMA_HASH_KEY(key) \
    carma_hash_bytes(CARMA_HASH_INIT, (const char*)&(key), sizeof(key))

#define CARMA_HASH_RANGE_KEY(key) \
    carma_hash_bytes(CARMA_HASH_INIT, (const char*)(BEGIN_POINTER(key)), COUNT_BYTES(key))

////////////////////////////////////////////////////////////////////////////////
// FIND DATA IN TABLE

#define FOR_EACH_TABLE(iterator, table) \
    for (CARMA_AUTO iterator = (table).data; iterator != (table).data + (table).capacity; ++iterator) \
        if ((iterator)->occupied)

#define CARMA_FIND_FREE_INDEX_FOR_KEY(table, k, _it) do { \
    size_t _capacity = (table).capacity; \
    CARMA_AUTO _base = CARMA_HASH_KEY(k) % _capacity; \
    bool _found = false; \
    for (size_t _offset = 0; _offset < _capacity; ++_offset) { \
        _it = (table).data + (_base + _offset) % _capacity; \
        if (!_it->occupied || _it->key == (k)) { \
            _found = true; \
            break; \
        } \
    } \
    CHECK_INTERNAL(_found, "Error in CARMA_FIND_FREE_INDEX_FOR_KEY "); \
} while (0)

#define CARMA_FIND_FREE_INDEX_FOR_RANGE_KEY(table, k, _it) do { \
    size_t _capacity = (table).capacity; \
    CARMA_AUTO _base = CARMA_HASH_RANGE_KEY(k) % _capacity; \
    bool _found = false; \
    for (size_t _offset = 0; _offset < _capacity; ++_offset) { \
        _it = (table).data + (_base + _offset) % _capacity; \
        if (!_it->occupied || ARE_EQUAL(_it->key, (k))) { \
            _found = true; \
            break; \
        } \
    } \
    CHECK_INTERNAL(_found, "Error in CARMA_FIND_FREE_INDEX_FOR_RANGE_KEY "); \
} while (0)

#define GET_KEY_VALUE(k, _value, table) do { \
    if (IS_EMPTY(table)) \
        break; \
    CARMA_AUTO _key = (k); \
    CARMA_AUTO _it = (table).data; \
    CARMA_FIND_FREE_INDEX_FOR_KEY((table), _key, _it); \
    if (_it->occupied) { \
        (_value) = _it->value; \
    } \
} while (0)

#define GET_RANGE_KEY_VALUE(_key, _value, table) do { \
    if (IS_EMPTY(table)) \
        break; \
    CARMA_AUTO _it = (table).data; \
    CARMA_FIND_FREE_INDEX_FOR_RANGE_KEY((table), (_key), _it); \
    if (_it->occupied) { \
        (_value) = _it->value; \
    } \
} while (0)

////////////////////////////////////////////////////////////////////////////////
// MODIFY TABLE

static inline
bool carma_is_power_of_two(size_t n) {
    // A power of two is greater than 0 and has only one bit set.
    // If n is a power of two then n - 1 will have all bits set,
    // below the single bit set in n, and n & (n - 1) will be 0.
    return (n != 0) && ((n & (n - 1)) == 0);
}

// Zero initializes with calloc so that occupied is false.
#define INIT_TABLE(table, mycapacity) do { \
    CHECK_INTERNAL(carma_is_power_of_two(mycapacity), "Table capacity should be a power of two"); \
    CARMA_CALLOC((table).data, (mycapacity)); \
    (table).count = (0); \
    (table).capacity = (mycapacity); \
} while (0)

#define FREE_TABLE(table) FREE_DARRAY(table)

#define CARMA_ENSURE_TABLE_CAPACITY_KEY(table) do { \
    if ((table).count + 1 < 0.7 * (table).capacity) { \
        break; \
    } \
    CARMA_AUTO new_capacity = CARMA_DOUBLED_CAPACITY((table).capacity); \
    CARMA_AUTO new_table = table; \
    INIT_TABLE(new_table, new_capacity); \
    new_table.count = (table).count; \
    FOR_EACH_TABLE(_old_item, (table)) { \
        CARMA_AUTO _new_item = new_table.data; \
        CARMA_FIND_FREE_INDEX_FOR_KEY((new_table), _old_item->key, _new_item); \
        *_new_item = *_old_item; \
    } \
    FREE_TABLE(table); \
    table = new_table; \
} while (0)

#define CARMA_ENSURE_TABLE_CAPACITY_RANGE_KEY(table) do { \
    if ((table).count + 1 < 0.7 * (table).capacity) { \
        break; \
    } \
    CARMA_AUTO new_capacity = CARMA_DOUBLED_CAPACITY((table).capacity); \
    CARMA_AUTO new_table = table; \
    INIT_TABLE(new_table, new_capacity); \
    new_table.count = (table).count; \
    FOR_EACH_TABLE(_old_item, (table)) { \
        CARMA_AUTO _new_item = new_table.data; \
        CARMA_FIND_FREE_INDEX_FOR_RANGE_KEY((new_table), _old_item->key, _new_item); \
        *_new_item = *_old_item; \
    } \
    FREE_TABLE(table); \
    table = new_table; \
} while (0)

#define SET_KEY_VALUE(k, v, table) do { \
    CARMA_ENSURE_TABLE_CAPACITY_KEY(table); \
    CARMA_AUTO _k = (k); \
    CARMA_AUTO _item = (table).data; \
    CARMA_FIND_FREE_INDEX_FOR_KEY((table), _k, _item); \
    if (!_item->occupied) { \
        (table).count++; \
    } \
    _item->key = _k; \
    _item->value = (v); \
    _item->occupied = (true); \
    CHECK_INTERNAL((table).count < (table).capacity, "There should always be room left in table"); \
} while (0)

#define SET_RANGE_KEY_VALUE(k, v, table) do { \
    CARMA_ENSURE_TABLE_CAPACITY_RANGE_KEY(table); \
    CARMA_AUTO _k = (k); \
    CARMA_AUTO _item = (table).data; \
    CARMA_FIND_FREE_INDEX_FOR_RANGE_KEY((table), _k, _item); \
    if (!_item->occupied) { \
        (table).count++; \
    } \
    _item->key = _k; \
    _item->value = (v); \
    _item->occupied = (true); \
    CHECK_INTERNAL((table).count < (table).capacity, "There should always be room left in table"); \
} while (0)

#define CLEAR_TABLE(table) FOR_EACH_TABLE(item, (table)) item->occupied = false;
