#pragma once

#include "carma_std.h"

#include "carma.h"

typedef struct TableIndices {
    size_t* data;
    size_t capacity;
} TableIndices;

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

#define CARMA_TABLE_EMPTY_INDEX_BYTE_PATTERN 0xFF

static inline
bool carma_is_slot_empty(size_t* index) {
    return *index == SIZE_MAX;
}

#define ARE_EQUAL_PRIMITIVES(a, b) ((a) == (b))

#define CARMA_FIND_INDEX_SLOT_FOR_GENERIC_KEY(table, k, _slot, hash_function, equality) do { \
    size_t _capacity = (table).indices.capacity; \
    CHECK_INTERNAL(_capacity, "Unexpected zero capacity"); \
    CARMA_AUTO _base = hash_function(k) % _capacity; \
    bool _found = false; \
    for (size_t _offset = 0; _offset < _capacity; ++_offset) { \
        (_slot) = (table).indices.data + (_base + _offset) % _capacity; \
        if (carma_is_slot_empty(_slot) || equality((table).items.data[*(_slot)].key, (k))) { \
            _found = true; \
            break; \
        } \
    } \
    CHECK_INTERNAL(_found, "Error in CARMA_FIND_INDEX_SLOT_FOR_GENERIC_KEY"); \
} while (0)

#define CARMA_FIND_INDEX_SLOT_FOR_KEY(table, k, _slot) \
    CARMA_FIND_INDEX_SLOT_FOR_GENERIC_KEY((table), (k), (_slot), CARMA_HASH_KEY, ARE_EQUAL_PRIMITIVES)

#define CARMA_FIND_INDEX_SLOT_FOR_RANGE_KEY(table, k, _slot) \
    CARMA_FIND_INDEX_SLOT_FOR_GENERIC_KEY((table), (k), (_slot), CARMA_HASH_RANGE_KEY, ARE_EQUAL)

#define GET_KEY_VALUE(k, _value, table) do { \
    if (IS_EMPTY((table).items)) \
        break; \
    CARMA_AUTO _key = (k); \
    CARMA_AUTO _slot = (table).indices.data; \
    CARMA_FIND_INDEX_SLOT_FOR_KEY((table), _key, _slot); \
    if (!carma_is_slot_empty(_slot)) { \
        (_value) = (table).items.data[*_slot].value; \
    } \
} while (0)

#define GET_RANGE_KEY_VALUE(_key, _value, table) do { \
    if (IS_EMPTY((table).items)) \
        break; \
    CARMA_AUTO _slot = (table).indices.data; \
    CARMA_FIND_INDEX_SLOT_FOR_RANGE_KEY((table), (_key), _slot); \
    if (!carma_is_slot_empty(_slot)) { \
        (_value) = (table).items.data[*_slot].value; \
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

#define CARMA_CLEAR_TABLE_INDICES(indices) do { \
    memset((indices).data, CARMA_TABLE_EMPTY_INDEX_BYTE_PATTERN, (indices).capacity * sizeof(*(indices).data)); \
} while (0)

#define INIT_TABLE(table, mycapacity) do { \
    CHECK_INTERNAL(carma_is_power_of_two(mycapacity), "Table capacity should be a power of two"); \
    INIT_DARRAY((table).items, 0, (mycapacity)); \
    CARMA_MALLOC((table).indices.data, (mycapacity)); \
    (table).indices.capacity = (mycapacity); \
    CARMA_CLEAR_TABLE_INDICES((table).indices); \
} while (0)

#define FREE_TABLE(table) do { \
    FREE_DARRAY((table).items); \
    free((table).indices.data); \
    (table).indices.data = NULL; \
    (table).indices.capacity = 0; \
} while (0)

#define CARMA_IS_ABOVE_LOAD_FACTOR(table) ((table).items.count + 1 >= 0.7 * (table).indices.capacity)

#define CARMA_INCREASE_TABLE_CAPACITY_KEY(table) do { \
    CARMA_AUTO _new_capacity = CARMA_DOUBLED_CAPACITY((table).indices.capacity); \
    free((table).indices.data); \
    CARMA_MALLOC((table).indices.data, _new_capacity); \
    (table).indices.capacity = _new_capacity; \
    CARMA_CLEAR_TABLE_INDICES((table).indices); \
    for (size_t _i = 0; _i < (table).items.count; ++_i) { \
        CARMA_AUTO _slot = (table).indices.data; \
        CARMA_FIND_INDEX_SLOT_FOR_KEY((table), (table).items.data[_i].key, _slot); \
        *_slot = _i; \
    } \
} while (0)

#define CARMA_INCREASE_TABLE_CAPACITY_RANGE_KEY(table) do { \
    CARMA_AUTO _new_capacity = CARMA_DOUBLED_CAPACITY((table).indices.capacity); \
    free((table).indices.data); \
    CARMA_MALLOC((table).indices.data, _new_capacity); \
    (table).indices.capacity = _new_capacity; \
    CARMA_CLEAR_TABLE_INDICES((table).indices); \
    FOR_INDEX(_i, (table).items) { \
        CARMA_AUTO _slot = (table).indices.data; \
        CARMA_FIND_INDEX_SLOT_FOR_RANGE_KEY((table), (table).items.data[_i].key, _slot); \
        *_slot = _i; \
    } \
} while (0)

#define SET_KEY_VALUE(k, v, table) do { \
    if (CARMA_IS_ABOVE_LOAD_FACTOR(table)) { \
        CARMA_INCREASE_TABLE_CAPACITY_KEY(table); \
    } \
    CARMA_AUTO _k = (k); \
    CARMA_AUTO _slot = (table).indices.data; \
    CARMA_FIND_INDEX_SLOT_FOR_KEY((table), _k, _slot); \
    if (carma_is_slot_empty(_slot)) { \
        *_slot = (table).items.count; \
        APPEND((table).items, MAKE(VALUE_TYPE((table).items), .key=_k, .value=(v))); \
    } else { \
        (table).items.data[*_slot].value = (v); \
    } \
} while (0)

#define SET_RANGE_KEY_VALUE(k, v, table) do { \
    if (CARMA_IS_ABOVE_LOAD_FACTOR(table)) { \
        CARMA_INCREASE_TABLE_CAPACITY_RANGE_KEY(table); \
    } \
    CARMA_AUTO _k = (k); \
    CARMA_AUTO _slot = (table).indices.data; \
    CARMA_FIND_INDEX_SLOT_FOR_RANGE_KEY((table), _k, _slot); \
    if (carma_is_slot_empty(_slot)) { \
        *_slot = (table).items.count; \
        APPEND((table).items, MAKE(VALUE_TYPE((table).items), .key=_k, .value=(v))); \
    } else { \
        (table).items.data[*_slot].value = (v); \
    } \
} while (0)

#define CLEAR_TABLE(table) do { \
    CLEAR((table).items); \
    CARMA_CLEAR_TABLE_INDICES((table).indices); \
} while (0)
